--TEST--
CLD2Encoding Tests
--DESCRIPTION--
Tests CLD2Encoding static methods and properties
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
var_dump(new CLD2Encoding());
var_dump(CLD2Encoding::UTF8);
var_dump(CLD2Encoding::UTF8UTF8);
var_dump(CLD2Encoding::SOFTBANK_ISO_2022_JP);
var_dump(CLD2Encoding::encodingName(CLD2Encoding::UTF8));
var_dump(CLD2Encoding::encodingName(74));
?>
--EXPECT--
object(CLD2Encoding)#1 (0) {
}
int(22)
int(63)
int(74)
string(4) "UTF8"
string(20) "SOFTBANK_ISO_2022_JP"