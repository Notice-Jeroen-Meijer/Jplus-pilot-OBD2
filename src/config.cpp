#include "config.h"
#include "CAN_config.h"

static CS_CONFIG_t cs_config;
static bool fetched = false;

void setConfigDefault_1 () {
  cs_config.magicnumber               = CONFIG_MAGIC_NUMBER;
  cs_config.version                   = 1;         // change if length of config changes
  cs_config.mode_serial               = 1;
  cs_config.mode_bluetooth            = 1;
  cs_config.mode_debug                = 0xf6;
  cs_config.mode_leds                 = 1;

  cs_config.can0_rx                   = GPIO_NUM_4;
  cs_config.can0_tx                   = GPIO_NUM_5;
  cs_config.can0_speed                = (uint16_t)CAN_SPEED_500KBPS;

  cs_config.command_handler           = NULL;
  cs_config.output_handler            = NULL;
  cs_config.boot_count                = 0;

  strcpy (cs_config.name_bluetooth,   "J+ pilot OBD2");
  strcpy (cs_config.pin_bluetooth,    "1234");     // not implemented in framework yet

  fetched = true;
}

// placeholder for initializing new fields in a new version of EEPROM config
//void setConfigDefault_2 () {
  //cs_config.version                   = 2;
//}

void setConfigDefault () {
  setConfigDefault_1 ();
  //setConfigDefault_2 ()
  // add more calls if needed
}

// Note we use the Serial object here as the serialhandler might not have been initialized.

// return a pointer to the config in RAM. Note that this is always a pointer and while different modules might
// individually call getConfig, it's only allocated once (static) and only the pointer is returned.
CS_CONFIG_t *getConfig () {
  if (fetched) return &cs_config;
  if (!EEPROM.begin (sizeof (CS_CONFIG_t))) {
    Serial.println ("failed to initialise EEPROM for reading");
    setConfigDefault (); // create a default config in RAM and hope for the best
    return &cs_config;
  }
  if (EEPROM.readBytes (0, &cs_config, sizeof (CS_CONFIG_t)) != sizeof (CS_CONFIG_t) || cs_config.magicnumber != 0x0caacee0) {
    Serial.println ("Not a valid EEPROM record");
    setConfigToEeprom (true);
  }
  // so here we have a valid cs_config
  //if (cs_config.version < 3) {
  //  Serial.println ("EEPROM structure changed");
  //  EEPROM.end ();
  //  setConfigDefault_x ();
    setConfigToEeprom (false);
  //} else {
    EEPROM.end ();
  //}
  return &cs_config;
}

void setConfigToEeprom (bool reset) {
  if (!EEPROM.begin (sizeof (CS_CONFIG_t)))
  {
    Serial.println ("failed to initialise EEPROM for writing");
    return;
  }
  if (reset) setConfigDefault_1 ();
  EEPROM.writeBytes (0, &cs_config, sizeof (CS_CONFIG_t));
  EEPROM.commit ();
  EEPROM.end ();
  return;
}
