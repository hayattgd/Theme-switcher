# Theme switcher

Theme switcher works by swapping folders, so it works on almost anything. This tool allows you to easily switch between different themes for your applications.

## Get started

### Installation

1. Make sure themesw is executable.
	```sh
	$ chmod +x /path/to/themesw
	```

2. To make a symbolic link for executable, run this:
	```sh
	$ ln -sf /path/to/themesw /usr/bin/themesw
	```

3. Create the necessary directories:
	```sh
	$ mkdir -p ~/.config/themesw/themes/
	```

4. Download dotfiles for your apps and place them in the appropriate directories.

Your `~/.config/themesw/` directory should look like this:
```
~/.config/themesw
├─ themes
│  ├─ your-theme
│  │  ├─ config
│  │  │  ├─ hypr
│  │  │  │  ├─ ...
│  │  │  ├─ kitty
│  │  │  ├─ mako
│  │  │  ├─ waybar
│  │  │  ├─ wofi
│  │  ├─ run.sh
```
`
- The `config` folder contains files to be copied to `~/.config`.

- `run.sh` is a script that runs after copying, useful for restarting apps to apply the theme.

### Usage

To switch themes, run the following command:
```sh
$ themesw apply your-theme
```

## License

This repository is distributed under [GNU GENERAL PUBLIC LICENSE](LICENSE)
