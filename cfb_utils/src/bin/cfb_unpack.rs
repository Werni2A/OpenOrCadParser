use cfb;
use std::path::{Path, PathBuf};
extern crate pathdiff;
use std::env;
use std::fs;
use std::fs::File;
use std::io::{Read, Write};
use std::process::exit;

fn help() {
    println!(
        "usage: cfb_unpack <input_cfb_file> <unpacked_cfb_root_dir>
Unpacks 'input_cfb_file' to 'unpacked_cfb_root_dir', where all all
'storages's become directories and all 'stream's become files."
    );
}

fn construct_external_path(cfb_path: &Path, fs_root_dir: &Path) -> PathBuf {
    // Strip `cfb_path` from leading `/`
    let rel_internal_path = Path::new(&cfb_path.to_str().unwrap()[1..]);
    let tmp_str_path = fs_root_dir.join(rel_internal_path);
    tmp_str_path
}

fn cfb_unpack(input_cfb: &Path, unpacked_cfb_dir: &Path) {
    println!("Unpacking: {:#?}", input_cfb);
    println!("Into:      {:#?}", unpacked_cfb_dir);

    let mut cfb = cfb::open(input_cfb).unwrap();

    fs::create_dir_all(&unpacked_cfb_dir).unwrap();

    let entries = cfb.walk().collect::<Vec<_>>();

    for entry in entries {
        let external_path = construct_external_path(entry.path(), unpacked_cfb_dir);

        if entry.is_storage() {
            println!("  Creating storage: {:#?}", external_path);
            fs::create_dir_all(&external_path).unwrap();
        }

        if entry.is_stream() {
            println!("  Creating stream:  {:#?}", external_path);

            let data = {
                let mut stream = cfb.open_stream(&entry.path()).unwrap();
                let mut buffer = Vec::new();
                stream.read_to_end(&mut buffer).unwrap();
                buffer
            };

            fs::create_dir_all(&external_path.parent().unwrap()).unwrap();
            let mut file = File::create(external_path).unwrap();
            // Write a slice of bytes to the file
            file.write_all(&data).unwrap();
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let mut input_cfb = Path::new("");
    let mut unpacked_cfb_dir = Path::new("");

    match args.len() {
        3 => {
            input_cfb = Path::new(&args[1]);
            unpacked_cfb_dir = Path::new(&args[2]);
        }
        _ => {
            help();
            exit(1);
        }
    }

    let tmp: PathBuf = input_cfb.canonicalize().unwrap();
    let input_cfb = Path::new(&tmp);

    cfb_unpack(input_cfb, unpacked_cfb_dir);
}
