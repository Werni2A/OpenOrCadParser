use cfb;
use std::env;
use std::path::{Path, PathBuf};
extern crate pathdiff;

use std::fs;
use std::io::Write;
use std::process::exit;

use walkdir::WalkDir;

fn help() {
    println!(
        "usage: cfb_pack <unpacked_cfb_root_dir> <output_cfb_file>
Generates 'output_cfb_file' from 'unpacked_cfb_root_dir', where all all
directories become 'storage's and all files become 'stream's."
    );
}

fn cfb_pack(output_cfb: &Path, unpacked_cfb_dir: &Path) {
    println!("Packing: {:#?}", unpacked_cfb_dir);
    println!("Into:    {:#?}", output_cfb);

    let mut cfb = cfb::create(output_cfb).unwrap();

    for entry in WalkDir::new(unpacked_cfb_dir) {
        let entry = entry.unwrap();

        let a = pathdiff::diff_paths(entry.path(), unpacked_cfb_dir).unwrap();
        let rel_path = a.as_path();

        let tmp_str = rel_path.to_str().unwrap().replace(r"\", "/");
        let mut internal_path = String::from("/");
        internal_path.push_str(&tmp_str);

        // Skip root storage
        if internal_path == "/" {
            continue;
        }

        if entry.metadata().unwrap().is_file() {
            println!("  Creating stream:  {:#?}", internal_path);
            let data: Vec<u8> = fs::read(entry.path()).unwrap();
            // @TODO: Need to create parent storages first. Otherwise create_stream fails
            let mut stream = cfb.create_stream(&internal_path).unwrap();
            stream.write_all(&data).unwrap();
        }

        if entry.metadata().unwrap().is_dir() {
            println!("  Creating storage: {:#?}", internal_path);
            cfb.create_storage_all(&internal_path).unwrap();
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();

    let mut output_cfb = Path::new("");
    let mut unpacked_cfb_dir = Path::new("");

    match args.len() {
        3 => {
            unpacked_cfb_dir = Path::new(&args[1]);
            output_cfb = Path::new(&args[2]);
        }
        _ => {
            help();
            exit(1);
        }
    }

    let tmp: PathBuf = unpacked_cfb_dir.canonicalize().unwrap();
    let unpacked_cfb_dir = Path::new(&tmp);

    cfb_pack(output_cfb, unpacked_cfb_dir);
}
