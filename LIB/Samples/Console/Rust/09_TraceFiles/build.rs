use std::env;

fn main(){
    let target = env::var("TARGET").unwrap();

    let lib_dir = if target.contains("x86_64") {
        "lib/x64"
    } else if target.contains("i686") {
        "lib/x86"
    } else {
        panic!("Unsupported target: {}", target);
    };

    println!("cargo:rustc-link-search=native={}", lib_dir);
    println!("cargo:rustc-link-lib=dylib=PCANBasic");
}