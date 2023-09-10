mod cli;
mod error;

use std::ffi::{c_char, CStr, CString};
use nix::unistd::{dup2, close, execvp, read, fork, ForkResult, pipe};
use nix::libc::{exit, atoll, EXIT_SUCCESS, STDOUT_FILENO, STDIN_FILENO, waitpid, c_int, PT_NULL};

use clap::Parser;
use crate::error::MyError;

const NUM_PROCS: usize = 9;

fn main() -> Result<(), MyError> {
    let settings = cli::Settings::parse();
    let c_filepath = CString::new(settings.file_path.clone())?;
    let total_rows = get_total_rows(&c_filepath)?;

    let formatted_awk = format!("BEGIN{{FS=\" \";nrows={}}}{{print $2\"\t\"$1\"\t\"$1/nrows*100\"%\"}}", total_rows);

    let commands = [
        ["/bin/cat", &settings.file_path, "", ""],
        ["/bin/awk", "BEGIN{FS=\" \"}($0 !~ /.*\"-\".*/){print $11}", "", ""],
        ["/bin/sed", "s/\"//g", "", ""],
        // ["/bin/sort", "-s", "", ""],
        // ["/bin/uniq", "-c", "", ""],
        // ["/bin/awk", &formatted_awk, "", ""],
        // ["/bin/sort", "-k", "3n", ""],
        // ["/bin/tail", "-10", "", ""],
        // ["/bin/tac", "", "", ""]
    ];
    let commands_args = commands
        .into_iter()
        .map(|args| args.into_iter()
            .map(|str| CString::new(str).unwrap())
            .collect::<Vec<CString>>()
        )
        .collect::<Vec<Vec<CString>>>();


    let num_procs = commands_args.len();
    let mut fds = Vec::new();
    for _ in 0..(num_procs - 1) {
        fds.push(pipe()?);
    }
    let mut pids = Vec::new();
    for i in 0..num_procs {
        unsafe { pids.push(fork()?) }
        match pids[i]  {
            ForkResult::Child => {
                if i == 0 {
                    dup2(fds[i].1, STDOUT_FILENO)?;
                } else if i == num_procs - 1 {
                    dup2(fds[i - 1].0, STDIN_FILENO)?;
                } else {
                    dup2(fds[i - 1].0, STDIN_FILENO)?;
                    dup2(fds[i].1, STDOUT_FILENO)?;
                }
                for p in &fds {
                    close(p.0)?;
                    close(p.1)?;
                }
                execvp(&commands_args[i][0], &commands_args[i])?;
                unsafe { exit(EXIT_SUCCESS) };
            },
            ForkResult::Parent { .. } => {}
        }
    }
    for p in &fds {
        close(p.0)?;
        close(p.1)?;
    }

    let mut status: c_int = 0;

    for (i, pi) in pids.iter().enumerate() {
        match pi {
            ForkResult::Parent { child, .. } => unsafe {
                if i == num_procs - 1 {
                    waitpid(child.as_raw(), &mut status as *mut c_int, 0);
                } else {
                    waitpid(child.as_raw(), 0 as *mut c_int, 0);
                }
            }
            ForkResult::Child => {}
        }
    }
    unsafe { exit(status) };
    Ok(())
}



fn get_total_rows(c_filepath: &CStr) -> Result<u128, MyError> {
    let fd = pipe()?;
    match unsafe{fork()?} {
        ForkResult::Child => unsafe {
            dup2(fd.1, nix::libc::STDOUT_FILENO)?;
            close(fd.0)?;
            close(fd.1)?;
            let args = vec![
                CStr::from_bytes_with_nul(b"/bin/awk\0")?,
                CStr::from_bytes_with_nul(b"END{print NR}\0")?,
                c_filepath,
                CStr::from_bytes_with_nul(b"\0")?
            ];
            execvp(args[0], &args)?;
            exit(EXIT_SUCCESS);
        }
        ForkResult::Parent { .. } => unsafe {
            let mut buf = [0u8; 255];
            read(fd.0, &mut buf)?;
            close(fd.0)?;
            close(fd.1)?;
            Ok(atoll(&buf as *const u8 as *const c_char) as u128)
        }
    }
}


