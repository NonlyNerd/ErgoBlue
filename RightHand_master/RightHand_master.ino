#include "shared.h"

// Types
using matrix_t = matrix_type<wscancode_t>;
using action_t = uint32_t;
using keystate = struct {
  scancode_t scanCode;
  bool down;
  uint32_t lastChange;
  action_t action;
};

// Globals
BLEDis dis;
BLEHidAdafruit hid;
BLEClientUart clientUart;
BLEBas battery;

hwstate leftSide{};
hwstate rightSide{};

// Declarations
void resetKeyMatrix();
void cent_connect_callback(uint16_t conn_handle);
void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason);
void scan_callback(ble_gap_evt_adv_report_t* report);
void startAdv();

void setup() {
  shared_setup();
  resetKeyMatrix();

  // Central and peripheral
  Bluefruit.begin(true, true);
  // Bluefruit.clearBonds();
  Bluefruit.autoConnLed(false);

  battery.begin();

  Bluefruit.setTxPower(0);
  Bluefruit.setName(BT_NAME);

  Bluefruit.Central.setConnectCallback(cent_connect_callback);
  Bluefruit.Central.setDisconnectCallback(cent_disconnect_callback);

  dis.setManufacturer(MANUFACTURER);
  dis.setModel(MODEL);
  dis.setHardwareRev(HW_REV);
  dis.setSoftwareRev(__DATE__);
  dis.begin();

  clientUart.begin();
  // clientUart.setRxCallback(cent_bleuart_rx_callback);

  /* Start Central Scanning
   * - Enable auto scan if disconnected
   * - Interval = 100 ms, window = 80 ms
   * - Filter only accept bleuart service
   * - Don't use active scan
   * - Start(timeout) with timeout = 0 will scan forever (until connected)
   */
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(BLEUART_UUID_SERVICE);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0); // 0 = Don't stop scanning after n seconds

  hid.begin();
  // delay(5000);
  // Bluefruit.printInfo();

  startAdv();
}

void cent_connect_callback(uint16_t conn_handle) {
  // TODO: Maybe make this more secure? I haven't looked into how secure this
  // is in the documentation :/
  char peer_name[32] = {0};
  Bluefruit.Gap.getPeerName(conn_handle, peer_name, sizeof(peer_name));

  DBG(Serial.print("[Cent] Connected to "));
  DBG(Serial.println(peer_name));

  if (clientUart.discover(conn_handle)) {
    // Enable TXD's notify
    clientUart.enableTXD();
  } else {
    Bluefruit.Central.disconnect(conn_handle);
  }

  resetKeyMatrix();
}

void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void)conn_handle;
  (void)reason;
  DBG(Serial.println("[Cent] Disconnected"));
  resetKeyMatrix();
}

void scan_callback(ble_gap_evt_adv_report_t* report) {
  // Check if advertising contain BleUart service
  if (Bluefruit.Scanner.checkReportForService(report, clientUart)) {
    // Connect to device with bleuart service in advertising
    Bluefruit.Central.connect(report);
  }
}

void startAdv(void) {
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.Advertising.addService(hid);

  Bluefruit.ScanResponse.addService(battery);

  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30); // number of seconds in fast mode
  Bluefruit.Advertising.start(0); // 0 = Don't stop advertising after n seconds
}

constexpr uint32_t kMask = 0xf00;
constexpr uint32_t kKeyPress = 0x100;
constexpr uint32_t kModifier = 0x200;
constexpr uint32_t kLayer = 0x300;
constexpr uint32_t kTapHold = 0x400;
constexpr uint32_t kToggleMod = 0x500;
constexpr uint32_t kKeyAndMod = 0x600;

#define PASTE(a, b) a##b

#define ___ 0
#define KEY(a) kKeyPress | PASTE(HID_KEY_, a)
#define MOD(a) kModifier | PASTE(KEYBOARD_MODIFIER_, a)
#define TMOD(a) kToggleMod | PASTE(KEYBOARD_MODIFIER_, a)
#define TAPH(a, b) \
  kTapHold | PASTE(HID_KEY_, a) | (PASTE(KEYBOARD_MODIFIER_, b) << 16)
#define KANDMOD(a, b) \
  kKeyAndMod | PASTE(HID_KEY_, a) | (PASTE(KEYBOARD_MODIFIER_, b) << 16)
#define LAYER(n) kLayer | n

// Left to right
#define R0(l0, l1, l2, l3, l4, l5, l6)
#define R1(l0, l1, l2, l3, l4, l5, l6)
#define R2(l0, l1, l2, l3, l4, l5)
#define R3(l0, l1, l2, l3, l4, l5, l6)
#define R4(l0, l1, l2, l3, l4)
// Left to right, top to bottom
#define TM(l0, l1, l2, l3, l4, l5)

#define KEYMAP(l00,                                                            \
               l01,                                                            \
               l02,                                                            \
               l03,                                                            \
               l04,                                                            \
               l05,                                                            \
               l10,                                                            \
               l11,                                                            \
               l12,                                                            \
               l13,                                                            \
               l14,                                                            \
               l15,                                                            \
               l20,                                                            \
               l21,                                                            \
               l22,                                                            \
               l23,                                                            \
               l24,                                                            \
               l25,                                                            \
               l30,                                                            \
               l31,                                                            \
               l32,                                                            \
               l33,                                                            \
               l34,                                                            \
               l35,                                                            \
               l40,                                                            \
               l41,                                                            \
               l42,                                                            \
               l43,                                                            \
               l44,                                                            \
               l45,                                                            \
               l50,                                                            \
               l51,                                                            \
               l52,                                                            \
               l53,                                                            \
               l54,                                                            \
               l55,                                                            \
               r00,                                                            \
               r01,                                                            \
               r02,                                                            \
               r03,                                                            \
               r04,                                                            \
               r05,                                                            \
               r10,                                                            \
               r11,                                                            \
               r12,                                                            \
               r13,                                                            \
               r14,                                                            \
               r15,                                                            \
               r20,                                                            \
               r21,                                                            \
               r22,                                                            \
               r23,                                                            \
               r24,                                                            \
               r25,                                                            \
               r30,                                                            \
               r31,                                                            \
               r32,                                                            \
               r33,                                                            \
               r34,                                                            \
               r35,                                                            \
               r40,                                                            \
               r41,                                                            \
               r42,                                                            \
               r43,                                                            \
               r44,                                                            \
               r45,                                                            \
               r50,                                                            \
               r51,                                                            \
               r52,                                                            \
               r53,                                                            \
               r54,                                                            \
               r55)                                                            \
  {                                                                            \
    l00, l01, l02, l03, l04, l05, r00, r01, r02, r03, r04, r05, l10, l11, l12, \
        l13, l14, l15, r10, r11, r12, r13, r14, r15, l20, l21, l22, l23, l24,  \
        l25, r20, r21, r22, r23, r24, r25, l30, l31, l32, l33, l34, l35, r30,  \
        r31, r32, r33, r34, r35, l40, l41, l42, l43, l44, l45, r40, r41, r42,  \
        r43, r44, r45, l50, l51, l52, l53, l54, l55, r50, r51, r52, r53, r54,  \
        r55                                                                    \
  }

keystate keyStates[16];
uint8_t layer_stack[8];
static uint8_t layer_pos = 0;

void resetKeyMatrix() {
  layer_pos = 0;
  layer_stack[0] = 0;
  memset(&leftSide, 0, sizeof(leftSide));
  memset(&rightSide, 0, sizeof(rightSide));
  memset(keyStates, 0xff, sizeof(keyStates));

  hid.keyRelease();
}

#if DEBUG
void printState(keystate* state) {
  Serial.print("ScanCode=");
  Serial.print(state->scanCode, HEX);
  Serial.print(" down=");
  Serial.print(state->down);
  Serial.print(" lastChange=");
  Serial.print(state->lastChange);
  Serial.print(" action=");
  Serial.print(state->action, HEX);
  Serial.println("");
}
#endif

keystate* stateSlot(uint8_t scanCode, uint32_t now) {
  keystate* vacant = nullptr;
  keystate* reap = nullptr;
  for (auto& s : keyStates) {
    if (s.scanCode == scanCode) {
      return &s;
    }
    if (!vacant && s.scanCode == 0xff) {
      vacant = &s;
      continue;
    }
    if (!s.down) {
      if (!reap) {
        reap = &s;
      } else if (now - s.lastChange > now - reap->lastChange) {
        // Idle longer than the other reapable candidate; choose
        // the eldest of them
        reap = &s;
      }
    }
  }
  if (vacant) {
    return vacant;
  }
  return reap;
}

const action_t keymap[][numcols * numrows * 2] = {
    // Layer 0
    KEYMAP(
        // LEFT
        KEY(1),
        KEY(2),
        KEY(3),
        ___ /* BLUE */,
        KANDMOD(C, LEFTGUI),
        MOD(LEFTALT),
        KEY(Q),
        KEY(W),
        KEY(E),
        KEY(4),
        KEY(5),
        MOD(LEFTGUI),
        KEY(A),
        KEY(S),
        KEY(D),
        KEY(R),
        KEY(T),
        KEY(TAB),
        KEY(Z),
        KEY(X),
        KEY(C),
        KEY(F),
        KEY(G),
        KEY(DELETE),
        KEY(BACKSLASH),
        KEY(MINUS),
        KEY(EQUAL),
        KEY(V),
        KEY(B),
        KEY(BACKSPACE),
        LAYER(1),
        KEY(BRACKET_LEFT),
        KEY(BRACKET_RIGHT),
        MOD(LEFTSHIFT),
        ___ /* REKT */,
        TAPH(ESCAPE, LEFTCTRL),

        // RIGHT
        MOD(RIGHTALT),
        KANDMOD(V, LEFTGUI),
        ___ /* RED */,
        KEY(8),
        KEY(9),
        KEY(0),
        MOD(RIGHTGUI),
        KEY(6),
        KEY(7),
        KEY(I),
        KEY(O),
        KEY(P),
        KEY(PAGE_UP),
        KEY(Y),
        KEY(U),
        KEY(K),
        KEY(L),
        KEY(SEMICOLON),
        KEY(PAGE_DOWN),
        KEY(H),
        KEY(J),
        KEY(COMMA),
        KEY(PERIOD),
        KEY(SLASH),
        KEY(SPACE),
        KEY(N),
        KEY(M),
        KEY(GRAVE),
        KEY(ARROW_UP),
        KEY(APOSTROPHE),
        MOD(RIGHTCTRL),
        KEY(RETURN),
        MOD(RIGHTSHIFT),
        KEY(ARROW_LEFT),
        KEY(ARROW_DOWN),
        KEY(ARROW_RIGHT)),

    // Layer 1
    KEYMAP(
        // LEFT
        KEY(F1),
        KEY(F2),
        KEY(F3),
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        KEY(F4),
        KEY(F5),
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,

        // RIGHT
        ___,
        ___,
        ___,
        KEY(F8),
        KEY(F9),
        KEY(F10),
        ___,
        KEY(F6),
        KEY(F7),
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___,
        ___)};

static uint32_t resolveActionForScanCodeOnActiveLayer(uint8_t scanCode) {
  int s = layer_pos;

  while (s >= 0 && keymap[layer_stack[s]][scanCode] == ___) {
    --s;
  }
  return keymap[layer_stack[s]][scanCode];
}
#if DEBUG
void dump(matrix_t& m) {
  Serial.println("Key Matrix:");
  for (int r = 0; r < numrows; r++) {
    for (int c = 0; c < numcols * 2; c++) {
      unsigned int mask = 1 << c;
      if (m.rows[r] & mask) {
        Serial.print("X ");
      } else {
        Serial.print("- ");
      }
    }
    Serial.println("");
  }
}
#endif

void loop() {
  uint32_t now = millis();

  hwstate downRight{now, rightSide};
  hwstate downLeft{clientUart, leftSide};

  if (downRight.battery_level != rightSide.battery_level ||
      downLeft.battery_level != leftSide.battery_level) {
    battery.notify((downRight.battery_level + downLeft.battery_level) / 2);
    rightSide.battery_level = downRight.battery_level;
    leftSide.battery_level = downLeft.battery_level;
  }
  bool keysChanged = downRight != rightSide || downLeft != leftSide;
  if (keysChanged) {
    DBG(downRight.dump());
    DBG(downLeft.dump());
  }
#if 0

  for (int rowNum = 0; rowNum < numrows; ++rowNum) {
    for (int colNum = 0; colNum < 2 * numcols; ++colNum) {
      auto scanCode = (rowNum * 2 * numcols) + colNum;
      auto isDown = down.rows[rowNum] & (1 << colNum);
      auto wasDown = lastRead.rows[rowNum] & (1 << colNum);

      if (isDown == wasDown) {
        continue;
      }
      keysChanged = true;

      auto state = stateSlot(scanCode, now);
      if (isDown && !state) {
        // Silently drop this key; we're tracking too many
        // other keys right now
        continue;
      }
      DBG(printState(state));

      bool isTransition = false;

      if (state) {
        if (state->scanCode == scanCode) {
          // Update the transition time, if any
          if (state->down != isDown) {
            state->lastChange = now;
            state->down = isDown;
            if (isDown) {
              state->action = resolveActionForScanCodeOnActiveLayer(scanCode);
            }
            isTransition = true;
          }
        } else {
          // We claimed a new slot, so set the transition
          // time to the current time.
          state->down = isDown;
          state->scanCode = scanCode;
          state->lastChange = now;
          if (isDown) {
            state->action = resolveActionForScanCodeOnActiveLayer(scanCode);
          }
          isTransition = true;
        }

        if (isTransition) {
          switch (state->action & kMask) {
            case kLayer:
              if (state->down) {
                // Push the new layer stack position
                layer_stack[++layer_pos] = state->action & 0xff;
              } else {
                // Pop off the layer stack
                --layer_pos;
              }
              break;
          }
        }
      }
    }
  }

  if (keysChanged) {
    DBG(dump(down));

    uint8_t report[6] = {0, 0, 0, 0, 0, 0};
    uint8_t repsize = 0;
    uint8_t mods = 0;

    for (auto& state : keyStates) {
      if (state.scanCode != 0xff && state.down) {
        switch (state.action & kMask) {
          case kTapHold:
            if (now - state.lastChange > 200) {
              // Holding
              mods |= (state.action >> 16) & 0xff;
            } else {
              // Tapping
              auto key = state.action & 0xff;
              if (key != 0 && repsize < 6) {
                report[repsize++] = key;
              }
            }
            break;
          case kKeyAndMod: {
            mods |= (state.action >> 16) & 0xff;
            auto key = state.action & 0xff;
            if (key != 0 && repsize < 6) {
              report[repsize++] = key;
            }
          } break;
          case kKeyPress: {
            auto key = state.action & 0xff;
            if (key != 0 && repsize < 6) {
              report[repsize++] = key;
            }
          } break;
          case kModifier:
            mods |= state.action & 0xff;
            break;
          case kToggleMod:
            mods ^= state.action & 0xff;
            break;
        }
      }
    }

#if DEBUG
    Serial.print("mods=");
    Serial.print(mods, HEX);
    Serial.print(" repsize=");
    Serial.print(repsize);
    for (int i = 0; i < repsize; i++) {
      Serial.print(" ");
      Serial.print(report[i], HEX);
    }
    Serial.println("");
#endif

    hid.keyboardReport(mods, report);
    lastRead = down;
  }
#endif
  waitForEvent(); // Request CPU enter low-power mode until an event occurs
}
