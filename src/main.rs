extern crate trickster;
extern crate endianness;
extern crate byteorder;
//use endianness::LittleEndian;
use byteorder::{ByteOrder, LittleEndian};
use endianness::{read_i32};
use trickster::Process;
use std::env;
use std::error::Error;
// use std::io::Read;

fn main() -> Result<(), Box<dyn Error>> {
    // Get command line arguments
    let args: Vec<String> = env::args().collect();
    if args.len() != 3 {
        println!("Usage: program <memory_address> <value>");
        return Ok(());
    }

    let memory_address: usize = args[1].parse().unwrap();
    let value: i32 = args[2].parse().unwrap();

    let ctx = Process::new("rw_memory_ex")?;

    println!();

    // Read memory at target address
    let read_buffer = ctx.read_memory::<i32>(memory_address)?.into_inner();
    let mut read_buffer_value = [0];
    byteorder::LittleEndian::read_i32_into(read_buffer.as_slice(), &mut read_buffer_value);
    println!("Value at memory address {} before write: {}", memory_address, read_buffer_value[0]);
    

    // Write value to target address
    let write_buffer = vec![(value & 0xff) as u8, ((value >> 8) & 0xff) as u8, ((value >> 16) & 0xff) as u8, ((value >> 24) & 0xff) as u8];
    ctx.write_memory::<u8>(memory_address, write_buffer)?;

    Ok(())
}
