use std::env;
use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::thread;
use byteorder::{BigEndian, ByteOrder};

fn handle_client(mut stream: TcpStream) {
    let mut buffer = [0; 8];
    stream.read_exact(&mut buffer).unwrap();
    let number = BigEndian::read_i64(&buffer);
    println!("Received: {}", number);
}

fn start_server(port: u16) {
    let listener = TcpListener::bind(("0.0.0.0", port)).unwrap();
    println!("Server listening on port {}", port);

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                thread::spawn(|| {
                    handle_client(stream);
                });
            }
            Err(e) => {
                eprintln!("Connection failed: {}", e);
            }
        }
    }
}

fn start_client(address: &str, port: u16, number: i64) {
    let mut stream = TcpStream::connect((address, port)).unwrap();
    let mut buffer = [0; 8];
    BigEndian::write_i64(&mut buffer, number);
    stream.write_all(&buffer).unwrap();
    println!("Sent: {}", number);
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 4 {
        eprintln!("Usage: {} <mode> <port> <neighbor_address> [number]", args[0]);
        return;
    }

    let mode = &args[1];
    let port: u16 = args[2].parse().unwrap();
    let neighbor_address = &args[3];

    match mode.as_str() {
        "server" => start_server(port),
        "client" => {
            if args.len() < 5 {
                eprintln!("Usage: {} client <port> <neighbor_address> <number>", args[0]);
                return;
            }
            let number: i64 = args[4].parse().unwrap();
            start_client(neighbor_address, port, number);
        }
        _ => eprintln!("Invalid mode. Use 'server' or 'client'."),
    }
}
