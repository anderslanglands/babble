use std::{path::{Path, PathBuf}, ffi::OsStr};
use regex::Regex;

#[cfg(target_os = "windows")]
fn print_link_args(dst: &Path, project_name: &str) {
    // get linker args from LinkLine.txt cmake output
    // TODO: wrap this all up in a crate
    let linkline_fn = format!("{}/build/{project_name}-link-libraries.txt", dst.display());
    let contents = std::fs::read_to_string(&linkline_fn)
        .unwrap_or_else(|e| panic!("could not read {linkline_fn}: {e}"));

    println!("cargo:rustc-link-search=native={}/build", dst.display());

    for token in contents.split_whitespace() {
        let path = Path::new(&token.replace('\\', "/")).to_owned();
        if let Some(parent) = path.parent() {
            if !parent.to_string_lossy().is_empty() {
                println!("cargo:rustc-link-search=native={}", parent.display());
            }
        }
        println!(
            "cargo:rustc-link-lib={}",
            path.file_stem().unwrap().to_string_lossy()
        );
    }
}

#[cfg(target_os = "linux")]
fn print_link_args(dst: &Path, project_name: &str) {
    // get linker args from LinkLine.txt cmake output
    // TODO: wrap this all up in a crate
    let linkline_fn = format!("{}/build/{project_name}-link-libraries.txt", dst.display());
    let contents = std::fs::read_to_string(&linkline_fn)
        .unwrap_or_else(|e| panic!("could not read {linkline_fn}: {e}"));

    println!("cargo:rustc-link-search=native={}/build", dst.display());

    for token in contents.split_whitespace() {
        let path = Path::new(&token.replace('\\', "/")).to_owned();
        if let Some(parent) = path.parent() {
            if !parent.to_string_lossy().is_empty() {
                println!("cargo:rustc-link-search=native={}", parent.display());
            }
        }

        // stem will be either "libsomething.a/.so.2.1" or "-lstdc++""
        let stem = path.file_stem().unwrap().to_string_lossy();

        let lib = if let Some(stripped) = stem.strip_prefix("-l") {
            stripped.to_string()
        } else if let Some(stripped) = stem.strip_prefix("lib") {
            if let Some(stripped) = stripped.strip_suffix(".a") {
                stripped.to_string()
            } else {
                let re = Regex::new(r"(.*)\.so[\.0-9]*|\.a").unwrap();
                if let Some(caps) = re.captures(stripped) {
                    caps[1].to_string()
                } else {
                    stripped.to_string()
                }
            }
        } else {
            panic!("unknown lib form \"{stem}\" from \"{}\"", path.display());
        };

        println!("cargo:rustc-link-lib={lib}");
    }
}

pub struct Config {
    project_name: String,
    cmake_config: cmake::Config
}

impl Config {
    pub fn new<P: AsRef<Path>>(project_name: &str, path: P) -> Config {
        Config {
            project_name: project_name.to_string(),
            cmake_config: cmake::Config::new(path)
        }
    }

    pub fn generator<S: AsRef<OsStr>>(&mut self, generator: S) -> &mut Config {
        self.cmake_config.generator(generator);
        self
    }

    pub fn profile(&mut self, profile: &str) -> &mut Config {
        self.cmake_config.profile(profile);
        self
    }

    pub fn build(&mut self) -> PathBuf {
        let dst = self.cmake_config.build();

        print_link_args(&dst, &self.project_name);

        dst
    }
}
