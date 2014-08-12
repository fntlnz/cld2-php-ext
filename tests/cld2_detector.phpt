--TEST--
CLD2Detector Tests
--FILE--
<?php
$cld2 = new CLD2Detector();
var_dump($cld2);
var_dump($cld2->isPlainText());
$cld2->setPlainText(true);
var_dump($cld2->isPlainText());
var_dump($cld2->getTldHint());
$cld2->setTldHint("com");
var_dump($cld2->getTldHint());
var_dump($cld2->getLanguageHint());
$cld2->setLanguageHint(CLD2Language::ITALIAN);
var_dump($cld2->getLanguageHint());
var_dump($cld2->getEncodingHint());
$cld2->setEncodingHint(CLD2Encoding::UTF8);
var_dump($cld2->getEncodingHint());
var_dump($cld2->detect('My name is Melissa'));
?>
--EXPECT--
object(CLD2Detector)#1 (4) {
  ["isPlainText"]=>
  bool(false)
  ["tldHint"]=>
  string(0) ""
  ["languageHint"]=>
  int(26)
  ["encodingHint"]=>
  int(23)
}
bool(false)
bool(true)
string(0) ""
string(3) "com"
int(26)
int(7)
int(23)
int(22)
array(5) {
  ["language_id"]=>
  int(0)
  ["language_code"]=>
  string(2) "en"
  ["language_name"]=>
  string(7) "ENGLISH"
  ["language_probability"]=>
  int(95)
  ["is_reliable"]=>
  bool(true)
}
