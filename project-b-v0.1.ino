/*
my second arduino project
GOAL: making an autonomous arduino code that gives you information about biology
written by tuana
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h> // make sure to use the right EEPROM lib for arduino otherwise it might break

#define DELAY_TIME 10000 // question timeout
#define BUZZER_PIN 8
#define LED_PIN 7
#define FACT_COUNT 32 // total facts (32 facts, thats a lot! :D)
// RAM ADDRESSES
#define CURRENT_INDEX_ADDR 33 // current fact goes to this address in eeprom for some purpose
#define INIT_FLAG_ADDR 34 // flag addr for initing eeprom once

LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd screen
// some variables that we need
const char* header = "Bunu biliyor muydunuz?"; // header, shows on startup once
String question = ""; // store the fact here to display and some other purposes
unsigned long int timer = 0; // timer! why not delay? because MY CODE STRUCTURE FUCKING SUCKS!
bool writeAnimation = true;
bool writeSound = true;

void(* resetFunc) (void) = 0; // reset arduino function (dunno how it works exactly. ai gave it to me.)

bool canWriteAnimation() {
  return writeAnimation;
}

bool canWriteSound() {
  return writeSound;
}

void setWriteAnimation(bool value) {
  writeAnimation = value;
}

void setWriteSound(bool value) {
  writeSound = value;
}

void handleIR(){
  // nah, im lazy
}

/* ========== Buzzer & LED ========== */
void playBuzzer(bool on = true) {
  digitalWrite(BUZZER_PIN, on); // pretty simple thx to C
}

void setup() {
  if (EEPROM.read(INIT_FLAG_ADDR) != 0xAA) { // check if eeprom isnt initalized
    initEEPROMAddresses(); // initalize eeprom
    EEPROM.update(INIT_FLAG_ADDR, 0xAA); // flagging to prevent data loss each start
  }

  pinMode(BUZZER_PIN, OUTPUT); // set BUZZER_PIN as an output pin
  pinMode(LED_PIN, OUTPUT); // bruh i should make a function for pinMode calls
  // *== COMPONENT CONTROL ==*
  lcd.init(); // initing lcd
  lcd.backlight(); // turn on lcd backlight
  parsePrint("The Ilginc Bilgies project", 50);
  delay(1000);
  parsePrint("Made with wires by Tuanapi.", 100);
  delay(500);
  parsePrint("Bilesenler kontrol ediliyor",80);
  delay(200); // waiting 200ms
  // ON
  playBuzzer(true); // if user hears a sound -> OK!
  digitalWrite(LED_PIN, HIGH); // if user gets blinded by the LED -> OK!
  delay(3000); // keep them running for 3s
  // OFF
  playBuzzer(false);
  digitalWrite(LED_PIN, LOW);
  delay(200); // too many delays... im sick of them but too lazy to use millis()
  parsePrint("Lutfen bekleyiniz...", 50); // print each char at 50ms
  delay(2000); // wait 2s
  parsePrint(header, 75); // print the header text chars at 75ms
  delay(2250); // wait for 2.25s

  validateQuestion(); // (1) decides on which fact to show under some condition
  attentionSequence(); // (2) trying to get attention with some led and buzzer (it should work!!!)
  timer = millis(); // set the timer to the current time (not lazy to implement this, it is easy)
}

void loop() {
  if(millis() - timer >= DELAY_TIME) { // yup, pretty simple logic
    timer = millis();
    validateQuestion(); // (3) then decide on the question, again (if it is not the last question) (if the last question, it will be handled in validateQuestion function)
    attentionSequence(); // (4) and again, try to get ppl attention with this (if it is not the last question)
    // damn i called the same shits four times, i will call them with a function instead but later, exam comes first (unfortunately)
  }
  //handleIR(); not today
}

/* ========== LCD Helpers ========== */
void prettyPrint(String text, unsigned short time) { // prints text with a simple writing animation
  if(canWriteAnimation()){
    for (int j = 0; j < text.length(); j++) { // loop is very important, very good for dynamic values. basically spins every text length times then
      lcd.print(text[j]); //prints the character (thanks to that 16x2 lcd's nature we dont have to store the text somewhere each loop)
      playBuzzer(canWriteSound());
      delay(time); // then wait and it spins again
      playBuzzer(false);
      }
      return; // early return to not call the code block below
  }
  lcd.print(text); // if canWriteAnimation() is false this code runs :(
  playBuzzer(canWriteSound());
  delay(time);
  playBuzzer(false);
}

void parsePrint(String txt, unsigned short time) { // prettyPrint's sister
  lcd.clear(); // clear screen
  lcd.setCursor(0, 0); // set cursor to col 0, ln 0
  prettyPrint(txt.substring(0, 16), time); // print the first 0-16 characters on that col, ln
  if(txt.length() > 16){ // if it is longer than 16
    lcd.setCursor(0, 1); // set ln to 1
  prettyPrint(txt.substring(16, 32), time); // write the rest of the message (16-32 only)
  // you can see how beautiful this function is
  }
}

void writeLCD(String text, unsigned short time = 200) { // just an alias of parsePrint, why did i even code this?
  parsePrint(text, time); // you know the thing already (THE MOST BEAUTIFUL SHIT EVER (!))
}

/* ========== EEPROM Initialization ========== */
void initEEPROMAddresses() {
  for (int i = 0; i < FACT_COUNT; i++) { // spin FACT_COUNT times (0 is not in use, it is üvey evlat hbajdjhasgdfbjasef)
    EEPROM.update(i, i + 1); // 1 = 0x01, 2 = 0x02, 3 = 0x03 and so on
  }
  EEPROM.update(CURRENT_INDEX_ADDR, 0x00); // reset current fact
}

void attentionSequence() { // ladies and gentlemans look at the lcd screen plsssss
  for (int i = 0; i < 3; i++) { // as u see this loop spin 3 times and it does... absolutetly nothing (!)
    playBuzzer(true); // play buzzer
    digitalWrite(LED_PIN, HIGH); // light up the led
    delay(500); // waits 0.5s
    playBuzzer(false); // turn off buzzer
    digitalWrite(LED_PIN, LOW); // and the led as well
    delay(500); // once again, wait for 0.5s then the loop continues
  }
}

/* ========== Idle Mode ========== */
void idleMode() { // tuana mode (sleep mode) shuuhs
  writeLCD("Soru bulunamadi!Lutfen bekleyinz", 90);
  delay(200);
  for (int i = 0; i < 15; i++) { // spin 15 times
    lcd.backlight(); // turn on the backlight of the lcd
    playBuzzer(true); // play buzzer
    delay(300); // wait 0.3s
    lcd.noBacklight(); // turn off backlight
    playBuzzer(false); // and buzzer as weel
    delay(300); // wait for another 0.3s then loop continues again and again (until no electrictşy :]) update: im adding auto reset! so it spins 15 times only
  } // after loop finishes:
  lcd.backlight();
  writeLCD("Reset atiliyor..", 50);
  resetFunc();
}

/* ========== Question Logic ========== */
void validateQuestion() { // very important one, the heart of this project (erm aksualy actually it is the 9v battery but shusssh caktırmayın!)
  bool found = false; // flag for checking whether a fact is been found or not

  for (int i = 0; i < FACT_COUNT && !found; i++) { // spining FACT_COUNT times under a condition (if u look at second param u will see) 
    byte val = EEPROM.read(i); // reading eeprom address i (smth dşfferebt each loop)
    if (val != 0xFF) { // checks if its null (impossible with my current code i suppose but anyway, i prfer having error handlers)
      switch (val) { // very smart move for saving up space from eeprom (its fkin 1kb like how tf can i fit 34099340kb in that thing)
        // AI GAVE ME THOSE INFORMATIONS - THEY MIGHT BE NOT CORRECT
        case 0x01: question = "Murekkep baligi renk degistirir."; break;
        case 0x02: question = "Tardigrat 30 yil uyuyabilir."; break;
        case 0x03: question = "Mantar plastikle beslenebilir."; break;
        case 0x04: question = "Denizanasi hic yaslanmaz."; break;
        case 0x05: question = "Vucut gunde 1L tukuruk uretir."; break;
        case 0x06: question = "Bakteri altin uretme yetenekli."; break;
        case 0x07: question = "Baliklar su altinda sarki soyler."; break;
        case 0x08: question = "Karinca mantar ciftligi kurar."; break;
        case 0x09: question = "Kurbaga mideyle dogum yapar."; break;
        case 0x0A: question = "Bitki muzikle hizli buyur."; break;
        case 0x0B: question = "Virus diger virusleri etkiler."; break;
        case 0x0C: question = "DNA'nin %8'i virus kodudur."; break;
        case 0x0D: question = "Bakteri uzayda yasayabilir."; break;
        case 0x0E: question = "Mantar radyasyonla beslenir."; break;
        case 0x0F: question = "Organizma super iletkendir."; break;
        case 0x10: question = "Vucut 300 milyar hucre uretir."; break;
        case 0x11: question = "Deniz canlisi isik uretir."; break;
        case 0x12: question = "Solucan bolunup cogalabilir."; break;
        case 0x13: question = "Bitki bocekleri sindirir."; break;
        case 0x14: question = "Beyin 20 watt ile calisir."; break;
        case 0x15: question = "Bakteri manyetik alan sezer."; break;
        case 0x16: question = "Yengec UV isigi algilar."; break;
        case 0x17: question = "Kuslar manyetik alan gorur."; break;
        case 0x18: question = "Vucutta 37T hucre bulunur."; break;
        case 0x19: question = "Mantar zombi bocek yaratir."; break;
        case 0x1A: question = "Balik karada yuruyebilir."; break;
        case 0x1B: question = "Organizma cam gibi seffaf."; break;
        case 0x1C: question = "Bakteri nukleer atik yer."; break;
        case 0x1D: question = "Bitki elektrikle iletisim kurar."; break;
        case 0x1E: question = "Virus kodu tersine cevirir."; break;
        case 0x1F: question = "Gozsuz canli gormeyi basarir."; break;
        case 0x20: question = "Organizma siviy azotta yasar."; break;
      }

      EEPROM.update(CURRENT_INDEX_ADDR, val); // with this we know where we are at even after resets. this just set addr CURRENT_INDEX_ADDR the current fact id
      EEPROM.update(i, 0xFF); // making it null so we wont use it again for now
      found = true; // and yh we founnd some gud shıt so we change this to true and loop breaks

      // if its the last question jst reset everything
      if (val == FACT_COUNT) {
        writeLCD(question, 90);
        delay(200);
        attentionSequence();
        delay(DELAY_TIME);
        initEEPROMAddresses(); // reset EEPROM slots
        resetFunc(); // restart Arduino
        return;
      }
    }
  }

  if (!found || question == "") { // check if no fact was found (might happen somehow, should catch whatever u can!)
    idleMode(); // No facts D:? goto tuana mode
  } else { // if found, act normal so others dont judge you (!)
    writeLCD(question, 90); // write the fact to the lcd
    delay(200);
  }
  
  /*
  written by tuana
  a school project
  */
}