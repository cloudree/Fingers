//
// Fingers v0.6
//  by cloudree
//
//  v0.0 : start for Dereste
//  v0.5 : for UNO, and re-maked
//  v0.51 : pattern : onegai cinderella master
//  v0.6 : Serial Managed

#include <Servo.h>
#include <String.h>

////////////////////////////////////////////////
// debug : output to serial monitor
#define SERIAL_DEBUG_LEVEL  1
//    0 = message
//    1 = pattern
//    2 = detail
#define SERIAL_SPEED    115200
#define SERIAL_DELAY    1

char dbgmsg[100];

void _serialmsg( int level, char *msg )
{
  if ( level <= SERIAL_DEBUG_LEVEL )
    Serial.print( msg );
}

void MSG( char *msg ) {
  _serialmsg( 0, msg );
}
void PAT( char *msg ) {
  _serialmsg( 1, msg );
}
void DBG( char *msg ) {
  _serialmsg( 2, msg );
}

////////////////////////////////////////////////
const int START_PIN =   2;
const int LED_PIN =     13;
const int SERVO_PIN =   3;   
// 13=LED
// 12=1LR, 11=1UD
// 10=2LR, 9=2UD
// 8=3LR, 7=3UD
// 6=4LR, 5=4UD
// 4=5LR, 3=5UD
// 2=START
// 1,0=Serial

const int FINGER_COUNT =      5;
const int SERVO_COUNT =       FINGER_COUNT;
const int SERVO_MIN_VALUE =   15;
const int SERVO_MAX_VALUE =   165;
const int SERVO_MID_VALUE =   ((SERVO_MIN_VALUE + SERVO_MAX_VALUE) / 2); // 90

const int ANGLE_UD_UP =       0;
const int ANGLE_UD_DOWN =     -8;

const int ANGLE_LR_LEFT =     8;
const int ANGLE_LR_CENTER =   0;
const int ANGLE_LR_RIGHT =    -8;

Servo svo_ud[ SERVO_COUNT ];
Servo svo_lr[ SERVO_COUNT ];
String readbuf;

const int mid_value_ud[ SERVO_COUNT ] = {
  // + UD -
  SERVO_MID_VALUE - 28,
  SERVO_MID_VALUE - 18,
  SERVO_MID_VALUE - 17,
  SERVO_MID_VALUE - 22,
  SERVO_MID_VALUE - 33
};

const int mid_value_lr[ SERVO_COUNT ] = {
  // + LR -
  SERVO_MID_VALUE - 6,
  SERVO_MID_VALUE - 4,
  SERVO_MID_VALUE + 7,
  SERVO_MID_VALUE - 3,
  SERVO_MID_VALUE + 6
};
  
enum state_finger {
  SF_IDLE = 0,
  SF_TOUCH_MOVE_DOWN, SF_TOUCH_DOWN, SF_TOUCH_MOVE_UP,
  SF_LONG_MOVE_DOWN, SF_LONG_DOWN, SF_LONG_MOVE_UP,
  SF_LEFT_MOVE_DOWN, SF_LEFT_MOVE, SF_LEFT_BACK,
  SF_RIGHT_MOVE_DOWN, SF_RIGHT_MOVE, SF_RIGHT_BACK,
  SF_MAX
} sf[ SERVO_COUNT ];

int state_delay_tick[ SF_MAX ] = {  // delay time for action
  0,
  50, 100, 50,
  50,   0, 50,     // 0 = infinite delay for long pattern
  50, 200, 50,
  50, 200, 50
};

long  sf_tick[ SERVO_COUNT ];     // wait until
unsigned long  start_tick = 0, last_tick = 0, play_tick = 0, step_tick = 0;

void finger_change( int f, state_finger s, int ud = ANGLE_UD_UP, int lr = ANGLE_LR_CENTER )
{
  svo_ud[ f ].write( mid_value_ud[ f ] + ud );
  svo_lr[ f ].write( mid_value_lr[ f ] + lr );

  sf[ f ] = s;
  int delay_tick = state_delay_tick[ s ];
  sf_tick[ f ] = ( delay_tick > 0 ) ? ( play_tick + delay_tick ) : 0;

  sprintf( dbgmsg, "[%08lu] --change f=%d, state=%d, ud=%d, lr=%d, d=%d, w=%08lu\n", play_tick / 100, f, s, ud, lr, delay_tick, sf_tick[f] / 100 );
  DBG( dbgmsg );
}

void finger_init( int f )
{
  sprintf( dbgmsg, "-init f=%d\n", f );
  DBG( dbgmsg );

  finger_change( f, SF_IDLE );
}

void finger_action( int f, int p )
{  
  switch ( p ) {
    case 0 : // no change
      break;

    case 1 : // touch sequence start
      finger_change( f, SF_TOUCH_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%08lu] TOUC %d\n", play_tick / 100, f );
      break;

    case 2 : // long-note sequence start
      finger_change( f, SF_LONG_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%08lu] DOWN %d\n", play_tick / 100, f );
      break;

    case 3 : // long_note finish action
      finger_change( f, SF_LONG_MOVE_UP, ANGLE_UD_UP );
      sprintf( dbgmsg, "[%08lu] UP   %d\n", play_tick / 100, f );
      break;

    case 4 : // left sequence start
      finger_change( f, SF_LEFT_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%08lu] LEFT %d\n", play_tick / 100, f );
      break;

    case 5 : // right sequence start
      finger_change( f, SF_RIGHT_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%08lu] RIGHT %d\n", play_tick / 100, f );
      break;
  }

  if ( p > 0 )
  {
    PAT( dbgmsg );
  }
}

void finger_process( int f )
{
  if ( sf_tick[ f ] <= 0 )  // long pattern
    return;

  if ( play_tick < sf_tick[ f ] )  // wait until
    return;

  switch ( sf[f] ) {
    case SF_IDLE:   break;

    // change to next state ...
    case SF_TOUCH_MOVE_DOWN:    finger_change( f, SF_TOUCH_DOWN, ANGLE_UD_DOWN );    break;
    case SF_TOUCH_DOWN:         finger_change( f, SF_TOUCH_MOVE_UP, ANGLE_UD_UP );   break;
    case SF_TOUCH_MOVE_UP:      finger_change( f, SF_IDLE );                         break;

    case SF_LONG_MOVE_DOWN:     finger_change( f, SF_LONG_DOWN, ANGLE_UD_DOWN );     break;
    case SF_LONG_DOWN:                                                              break;    // infinite loop until up
    case SF_LONG_MOVE_UP:       finger_change( f, SF_IDLE );                         break;

    case SF_LEFT_MOVE_DOWN:     finger_change( f, SF_LEFT_MOVE, ANGLE_UD_DOWN, ANGLE_LR_LEFT );    break;
    case SF_LEFT_MOVE:          finger_change( f, SF_LEFT_BACK );    break;
    case SF_LEFT_BACK:          finger_change( f, SF_IDLE );         break;

    case SF_RIGHT_MOVE_DOWN:    finger_change( f, SF_RIGHT_MOVE, ANGLE_UD_DOWN, ANGLE_LR_RIGHT );  break;
    case SF_RIGHT_MOVE:         finger_change( f, SF_RIGHT_BACK );   break;
    case SF_RIGHT_BACK:         finger_change( f, SF_IDLE );         break;
  }
}

////////////////////////////////////////////////
void setup()
{
  // serial debugger
  Serial.begin( SERIAL_SPEED );
  Serial.setTimeout( SERIAL_DELAY ); 
  while ( !Serial )
  {
    ; // wait for serial connection
  }

  // setup pins
  pinMode( START_PIN, INPUT );
  pinMode( LED_PIN, OUTPUT );
  for ( int i = 0; i < SERVO_COUNT; i ++ )
    svo_ud[i].attach( SERVO_PIN + i * 2 );      // 3,5,7,9,11
  for ( int i = 0; i < SERVO_COUNT; i ++ )
    svo_lr[i].attach( SERVO_PIN + i * 2 + 1);   // 4,6,8,10,12

  // reset positions
  for ( int i = 0; i < SERVO_COUNT; i ++ )
  {
    finger_init( i );
  }

  // init vals
  last_tick = start_tick = millis();
  play_tick = step_tick = 0;
  readbuf = "";

  // ready
  MSG( "Ready to Start\n" );
}

///////////////////////////////////////////////
/*
  void test_loop()
  {
  // restart and wait
  for ( int i = 0; i < SERVO_COUNT; i ++ )
    svo_ud[i].write( mid_value_ud[i] - 10 );
  delay( 500 );

  for ( int i = 0; i < SERVO_COUNT; i ++ )
    svo_ud[i].write( mid_value_ud[i] + 10 );
  delay( 500 );
  }
*/

////////////////////////////////////////////////
void loop()
{
  unsigned long cur_tick = millis();
  if ( cur_tick > last_tick ) {
    play_tick += cur_tick - last_tick;
    step_tick += cur_tick - last_tick;
    last_tick = cur_tick;
  }
  
  for ( int i = 0; i < SERVO_COUNT; i++ )
    finger_process( i );

  // read from serial
  int rd = Serial.read();
  if( rd < '0' || '9' < rd )
    return;
  readbuf.concat( rd-'0' );
  if( readbuf.length() < SERVO_COUNT )
    return;

  sprintf( dbgmsg, "[%08lu] recv = %s", play_tick / 100, readbuf.c_str() );
  PAT( dbgmsg );

  // move to next step
  for ( int i = 0; i < SERVO_COUNT; i++ ) {
    finger_action( i, readbuf[i] - '0');
    finger_process( i );
  }
  
  readbuf = "";
}

