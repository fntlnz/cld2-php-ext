--TEST--
CLD2Language Tests
--DESCRIPTION--
Tests CLD2Language static methods and properties
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
var_dump(CLD2Language::ITALIAN);
var_dump(CLD2Language::languageName(7));
var_dump(CLD2Language::languageName(CLD2Language::ENGLISH));
var_dump(CLD2Language::languageCode(CLD2Language::PAMPANGA));
var_dump(CLD2Language::getLanguageFromName('PAMPANGA'));
var_dump(CLD2Language::getLanguageFromName('pam'));
var_dump(CLD2Language::getLanguageFromName('en-Latn-GB'));
var_dump(CLD2Language::languageName(1000002020));
var_dump(CLD2Language::languageCode(-1));
?>
--EXPECT--
int(7)
string(7) "ITALIAN"
string(7) "ENGLISH"
string(3) "pam"
int(176)
int(176)
int(0)
string(7) "Unknown"
string(2) "un"
