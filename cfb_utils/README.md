# CFB Utils

Rust based CLI tools to extract CFB containers and rebuild them. Streams become files and storages become directories while unpacking, packing behaves vice versa.

## Usage

```bash
cargo build

# Unpack your CFB file
cargo run --bin cfb_unpack YOUR_CFB.DSN ./output_dir

# Modify streams

# Pack the folder to a CFB file again
cargo run --bin cfb_pack ./output_dir YOUR_CFB2.DSN
```
