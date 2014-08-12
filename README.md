## Compact Language Detector 2 - php ext


## Usage

### Detector

```php
$cld2 = new \CLD2Detector();

$cld2->setTldHint('it'); // optional, hints about the Top level domain (it: italian, fr: french, de: german etc..)
$cld2->setLanguageHint(CLD2Language::GERMAN); // optional, hints about the language.
$cld2->setEncodingHint(CLD2Encoding::UTF8); // optional, hints about text encoding
 
var_dump($cld2->detect('My name is Melissa'));

// Output
array(5) {
  'language_id' =>
  int(0)
  'language_code' =>
  string(2) "en"
  'language_name' =>
  string(7) "ENGLISH"
  'language_probability' =>
  int(95)
  'is_reliable' =>
  bool(true)
}

```


### Language

Get language name from Language id
```php
$lang = CLD2Language::PAMPANGA;
echo CLD2Language::languageName($lang);

// Output
string(8) "PAMPANGA"
```

Get language code from Language id
```php
echo CLD2Language::languageCode(CLD2Language::PAMPANGA);

// Output
string(3) "pam"
```

Get language id from name where Name can be either full name or ISO code, or can be ISO code embedded in a language-script combination such as "en-Latn-GB"
```php
echo CLD2Language::getLanguageFromName('PAMPANGA');
// or
echo CLD2Language::getLanguageFromName('pam');

// Output
int(176)
``` 
 
### Encoding

Get encoding name from id
```php
$enc = CLD2Encoding::UTF8;
echo CLD2Encoding::encodingName($enc);

// Output
string(4) "UTF8"
```

## Installation

## From PECL
Not submitted yet.

## Build from source

First of all clone current repository and enter into its directory.

```bash
git clone git@github.com:fntlnz/cld2-php-ext.git
cd cld2-php-ext
```

**Compile CLD2 library**

```bash
git svn clone http://cld2.googlecode.com/svn/trunk/ libcld2
cd libcld2/internal
./compile_libs.sh
sudo cp libcld2.so /usr/local/lib 
```

**Compile CLD2 PHP extension**

Come back to the `cld2-php-ext` directory and execute:

```bash
phpize
./configure --with-cld2=libcld2
make -j
sudo make install
```

Do not forget to add `extension=cld2.so` to your PHP ini.


## Contribute

To contribute you need [PHP built for extension development](http://php.net/manual/en/internals2.buildsys.environment.php),
For this purpose you can use the Dockerfile distributed with the project.
Using that Dockerfile, these are the steps:

Replace `~/Projects` with your projects directory

- Build the container (only the first time)

    ```bash
    git clone git@github.com:fntlnz/cld2-php-ext.git ~/Projects/cld2-php-ext 
    cd ~/Projects/cld2-php-ext
    docker build -t fntlnz/cld2-php-ext .
    ```

- Do your changes...

- Test your changes.

    ```bash
    docker  run --rm -v ~/Projects/cld2-php-ext:/cld2-php-ext fntlnz/cld2-php-ext "echo CLD2Language::languageName(CLD2Language::ITALIAN);"
    ```

## NOTES

- [CLD2 library](https://code.google.com/p/cld2)
- [CLD2 full version](https://code.google.com/p/cld2/wiki/CLD2FullVersion)
