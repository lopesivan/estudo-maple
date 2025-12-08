#!/usr/bin/env bash

JAVA=/opt/maple2021/jre.X86_64_LINUX/bin/java
JAVAC=/opt/maple2021/jre.X86_64_LINUX/bin/javac

${JAVAC} -cp "/opt/maple2021/java/externalcall.jar:/opt/maple2021/java/Maple.jar" test.java

PATH=/opt/maple2021/bin.X86_64_LINUX:$PATH \
    LD_LIBRARY_PATH=/opt/maple2021/bin.X86_64_LINUX \
    MAPLE=/opt/maple2021 \
    ${JAVA} \
    -cp ".:/opt/maple2021/java/externalcall.jar:/opt/maple2021/java/Maple.jar" \
    test

exit 0
