use std::error;

/// A simple wrapper around Result to box the type.
pub trait ResultBoxed<T, E>: Sized
    where E: Into<Box<dyn error::Error>>
{
    fn boxed(self) -> Result<T, Box<dyn error::Error>>;
}

impl<T, E> ResultBoxed<T, E>
    for Result<T, E>
    where E: Into<Box<dyn error::Error>>
{
    fn boxed(self) -> Result<T, Box<dyn error::Error>> {
        self.map_err(|err| err.into())
    }
}
