#[derive(Debug, thiserror::Error)]
pub enum MyError {
    #[error(transparent)]
    Errno(#[from] nix::errno::Errno),
    #[error(transparent)]
    FromBytesWithNul(#[from] core::ffi::FromBytesWithNulError),
    #[error(transparent)]
    Nul(#[from] std::ffi::NulError)
}