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

```
cd libcld2/internal
./compile_libs.sh
```

### Compile extension
```
phpize
make -j
```
and install
```
make install
```

## TODO:
- [ ] Externalize libcld2
- [ ] Allow output of languages probability vector  

## NOTES
libcld2: https://code.google.com/p/cld2/
For now libcld2 is included with the extension source.
