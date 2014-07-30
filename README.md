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

### Compile libcld2

```bash
git svn clone http://cld2.googlecode.com/svn/trunk/ libcld2
cd libcld2/internal
./compile_libs.sh
cp libcld2.so /usr/local/lib 
```

### Compile extension

```bash
phpize
./configure --with-cld2=libcld2
make -j
make install
```

## NOTES
- libcld2: https://code.google.com/p/cld2/
- CLD2FullVersion: https://code.google.com/p/cld2/wiki/CLD2FullVersion
