# Contributing

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
