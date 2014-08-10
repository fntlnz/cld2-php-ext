FROM fntlnz/php-debug:latest

RUN yum install -y git git-svn

RUN git svn clone http://cld2.googlecode.com/svn/trunk/ /libcld2
WORKDIR /libcld2/internal
RUN ./compile_libs.sh
RUN cp libcld2.so /usr/local/lib


VOLUME ['/cld2-php-ext']
WORKDIR /cld2-php-ext


RUN echo '#!/bin/bash' >> /usr/local/bin/run.sh
RUN echo 'phpize --clean && phpize && ./configure --with-cld2=/libcld2 && make && make install' >> /usr/local/bin/run.sh
RUN echo '/usr/local/bin/php -d extension=cld2.so -r "$@"' >> /usr/local/bin/run.sh
RUN chmod +x /usr/local/bin/run.sh


ENTRYPOINT ["run.sh"]