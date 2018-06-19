/***** XBEE APPLICATION PROJECT *****
 *
 * Auto-generated header with information about the
 * relation between the XBee module pins and the
 * project components.
 *
 ************ XBEE LAYOUT ***********
 *
 * This layout represents the XBee ZB (S2B) module
 * selected for the project with its pin distribution:
 *               _________________
 *              /     ________    \
 *             /     |   __   |    \
 *            /      | //  \\ |     \
 *   XPIN1  -|       | \\__// |      |- XPIN20
 *   XPIN2  -|       |________|      |- XPIN19
 *   XPIN3  -|                       |- XPIN18
 *   XPIN4  -| ===================== |- XPIN17
 *   XPIN5  -| #   # ####  #### #### |- XPIN16
 *   XPIN6  -|  # #  #   # #    #    |- XPIN15
 *   XPIN7  -|   #   ####  ###  ###  |- XPIN14
 *   XPIN8  -|  # #  #   # #    #    |- XPIN13
 *   XPIN9  -| #   # ####  #### #### |- XPIN12
 *   XPIN10 -| ===================== |- XPIN11
 *           |_______________________|
 *
 ************ PINS LEGEND ***********
 *
 * The following list displays all the XBee Module pins
 * with the project component which is using each one:
 *
 *   XPIN1 = VCC
 *   XPIN2 = uart0 [TX Pin]
 *   XPIN3 = uart0 [RX Pin]
 *   XPIN4 = <<UNUSED>>
 *   XPIN5 = special0 [Reset Pin]
 *   XPIN6 = special0 [RSSI PWM Pin]
 *   XPIN7 = LED [GPIO Pin]
 *   XPIN8 = special0 [BKGD Pin]
 *   XPIN9 = <<UNUSED>>
 *   XPIN10 = GND
 *   XPIN11 = <<UNUSED>>
 *   XPIN12 = <<UNUSED>>
 *   XPIN13 = <<UNUSED>>
 *   XPIN14 = VCC REF
 *   XPIN15 = special0 [Association Pin]
 *   XPIN16 = RELAY_2 [GPIO Pin]
 *   XPIN17 = RELAY [GPIO Pin]
 *   XPIN18 = <<UNUSED>>
 *   XPIN19 = LED_GROVE
 *   XPIN20 = special0 [Commissioning Pin]
 *
 ************************************/

#include <xbee_config.h>
#include <types.h>

void main(void)
{
  uint8_t option;

  sys_hw_init();
  sys_xbee_init();
  //sys_app_banner();


  sys_watchdog_reset();
  sys_xbee_tick();

  sys_reset(APP_CAUSE_BYPASS_MODE);
}
