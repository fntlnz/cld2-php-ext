# Chrome Language Detect PHP Extension


## Usage

```php
$cld2 = new CLD2();
echo $cld2->detect("My name is Melissa");
// output: ENGLISH
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
