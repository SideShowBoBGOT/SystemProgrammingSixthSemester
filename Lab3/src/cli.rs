use std::path::PathBuf;
use clap::Parser;

#[derive(Parser, Debug)]
pub struct Settings {
    #[arg(short, long)]
    pub file_path: String,
}