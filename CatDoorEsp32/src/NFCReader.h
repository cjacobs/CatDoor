#pragma once

#include <Adafruit_PN532.h>
#include <Arduino.h>
#include <SPI.h>

class NFCReader
{
  private:
    Adafruit_PN532 nfc;
    bool isValid = false;

  public:
    NFCReader(int pin) : nfc(pin)
    {
    }

    bool valid()
    {
        return isValid;
    }

    void begin()
    {
        nfc.begin();
        isValid = checkNFCInterface(true);

        // Set the max number of retry attempts to read from a card
        // This prevents us from waiting forever for a card, which is
        // the default behaviour of the PN532.
        // if (isValid)
        // {
        //     nfc.setPassiveActivationRetries(0xFF);
        // }
    }

    bool checkNFCInterface(bool verbose = false)
    {
        uint32_t versiondata = nfc.getFirmwareVersion();
        if (!versiondata)
        {
            return false;
        }

        if (verbose)
        {
            // Got ok data, print it out
            Serial.print("Found chip PN5");
            Serial.println((versiondata >> 24) & 0xFF, HEX);
            Serial.print("Firmware ver. ");
            Serial.print((versiondata >> 16) & 0xFF, DEC);
            Serial.print('.');
            Serial.println((versiondata >> 8) & 0xFF, DEC);
        }

        return true;
    }

    uint8_t readTag(uint8_t uid[], uint8_t *uidLength, uint16_t timeout = 0)
    {
        if (!valid())
            return 0;

        uint32_t versiondata = nfc.getFirmwareVersion();
        if (!versiondata)
            return 0;

        // Wait for an NTAG203 card.  When one is found 'uid' will be populated with
        // the UID, and uidLength will indicate the size of the UUID (normally 7)
        // uint16_t timeout = 0;

        uint8_t success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, uidLength, timeout);
        return success;
    }

    void processTag(uint8_t uid[], uint8_t uidLength)
    {
        if (!valid())
            return;
        // TODO: check for correct tag

        // Display some basic information about the card
        Serial.println("Found an ISO14443A card");
        Serial.print("  UID Length: ");
        Serial.print(uidLength, DEC);
        Serial.println(" bytes");
        Serial.print("  UID Value: ");
        nfc.PrintHex(uid, uidLength);
        Serial.println("");

        if (uidLength == 7)
        {
            uint8_t data[32];

            // We probably have an NTAG2xx card (though it could be Ultralight as
            // well)
            Serial.println("Seems to be an NTAG2xx tag (7 byte UID)");

            // NTAG2x3 cards have 39*4 bytes of user pages (156 user bytes),
            // starting at page 4 ... larger cards just add pages to the end of
            // this range:

            // See: http://www.nxp.com/documents/short_data_sheet/NTAG203_SDS.pdf

            // TAG Type       PAGES   USER START    USER STOP
            // --------       -----   ----------    ---------
            // NTAG 203       42      4             39
            // NTAG 213       45      4             39
            // NTAG 215       135     4             129
            // NTAG 216       231     4             225

            for (uint8_t i = 0; i < 42; i++)
            {
                uint8_t success = nfc.ntag2xx_ReadPage(i, data);

                // Display the current page number
                Serial.print("PAGE ");
                if (i < 10)
                {
                    Serial.print("0");
                    Serial.print(i);
                }
                else
                {
                    Serial.print(i);
                }
                Serial.print(": ");

                // Display the results, depending on 'success'
                if (success)
                {
                    // Dump the page data
                    nfc.PrintHexChar(data, 4);
                }
                else
                {
                    // Serial.println("Unable to read the requested page!");
                }
            }
        }
    }
};
