## Compact Language Detector 2 - php ext


## Usage

```php
$cld2 = new CLD2();
$lang = $cld2->detect('My name is Melissa');
// Output
array(4) {
  ["language_code"]=>
  string(2) "en"
  ["language_name"]=>
  string(7) "ENGLISH"
  ["language_probability"]=>
  int(95)
  ["is_reliable"]=>
  bool(true)
}

```

## Installation

First of all clone current repository and enter into its directory.

```bash
git clone git@github.com:fntlnz/cld2-php-ext.git
cd cld2-php-ext
```

### Compile CLD2 library

```bash
svn checkout http://cld2.googlecode.com/svn/trunk/ libcld2
cd libcld2/internal
./compile_libs.sh
sudo cp libcld2.so /usr/local/lib 
```

### Compile CLD2 PHP extension

Come back to the `cld2-php-ext` directory and execute:

```bash
phpize
./configure --with-cld2=libcld2
make -j
sudo make install
```

Do not forget to add `extension=cld2.so` to your PHP ini.

## NOTES

- [CLD2 library](https://code.google.com/p/cld2)
- [CLD2 full version](https://code.google.com/p/cld2/wiki/CLD2FullVersion)