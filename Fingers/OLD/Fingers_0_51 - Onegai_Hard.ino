//
// Fingers v0.51
//  by cloudree
//
//  v0.0 : start for Dereste
//  v0.5 : for UNO, and re-maked
//  v0.51 : pattern : onegai cinderella master

#include <Servo.h>
#include <String.h>

////////////////////////////////////////////////
// debug : output to serial monitor
#define SERIAL_DEBUG_LEVEL    1
//    0 = message
//    1 = pattern
//    2 = detail
#define SERIAL_SPEED    115200

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
const int SERVO_PIN =   12;

const int SERVO_COUNT =       5;
const int SERVO_MIN_VALUE =   15;
const int SERVO_MAX_VALUE =   165;
const int SERVO_MID_VALUE =   ((SERVO_MIN_VALUE + SERVO_MAX_VALUE) / 2); // 90

const int ANGLE_UD_UP =       0;
const int ANGLE_UD_DOWN =     -8;

const int ANGLE_LR_LEFT =     -3;
const int ANGLE_LR_CENTER =   0;
const int ANGLE_LR_RIGHT =    3;

Servo svo_ud[ SERVO_COUNT ];
Servo svo_lr[ SERVO_COUNT ];

const int mid_value_ud[ SERVO_COUNT ] = {
  // UD
  SERVO_MID_VALUE - 27,
  SERVO_MID_VALUE - 17,
  SERVO_MID_VALUE - 13,
  SERVO_MID_VALUE - 18,
  SERVO_MID_VALUE - 17
};

const int mid_value_lr[ SERVO_COUNT ] = {
  // LR
  SERVO_MID_VALUE - 9,
  SERVO_MID_VALUE - 4,
  SERVO_MID_VALUE + 8,
  SERVO_MID_VALUE + 0,
  SERVO_MID_VALUE + 10
};

////////////////////////////////////////////////
// pattern
//   time, F1, F2, F3, F4, F5
//
// * time = -1 : end
// * Fx is state :
//    0 = no change
//    1 = touch (down & up)
//    2 = down for long note
//    3 = up for long note
//    4 = left shift
//    5 = right shift

const float spd = 45.57;   // pattern speed (ms), big value is slower

const char p[] = {

  /*
    // test pattern
    0, 2, 0, 0, 0, 0,
    10, 0, 2, 0, 0, 0,
    10, 3, 0, 2, 0, 0,
    10, 0, 3, 0, 2, 0,
    10, 0, 0, 3, 0, 2,
    10, 0, 0, 0, 3, 0,
    10, 0, 0, 0, 0, 3,
  */

  // onegai cindelela master : https://www.youtube.com/watch?v=N2tYY_AL5Uc
  0, 5, 0, 0, 0, 0, //0
  1, 0, 5, 0, 0, 0,
  1, 0, 0, 5, 0, 0,
  1, 0, 0, 0, 5, 0,
  1, 0, 0, 0, 0, 5,
  8, 1, 0, 0, 0, 0, //5
  4, 0, 1, 0, 0, 0,
  4, 0, 0, 0, 1, 0,
  8, 0, 0, 0, 0, 2,
  8, 0, 0, 0, 0, 3,
  4, 1, 0, 0, 0, 0, //10-11
  4, 0, 1, 0, 0, 0,
  4, 0, 0, 0, 0, 1,
  16, 1, 0, 0, 0, 0,
  4, 0, 0, 0, 1, 0,
  4, 0, 0, 0, 0, 1, //15
  4, 1, 0, 0, 0, 0,
  8, 0, 1, 0, 0, 0,
  4, 0, 0, 0, 1, 0,
  4, 0, 0, 0, 0, 1,
  4, 0, 0, 0, 1, 0, //20-21
  4, 0, 2, 0, 0, 0,
  8, 0, 4, 0, 0, 0,
  8, 1, 0, 0, 0, 0,
  4, 0, 1, 0, 0, 0,
  2, 0, 1, 0, 0, 0, //25
  4, 0, 0, 0, 1, 0,
  4, 0, 0, 0, 0, 2,
  8, 0, 0, 0, 0, 3,
  4, 1, 0, 0, 0, 0,
  4, 0, 0, 2, 0, 0, //30-31
  16, 0, 0, 3, 0, 0,
  4, 1, 0, 0, 0, 0,
  4, 1, 0, 0, 0, 0,
  4, 0, 0, 0, 0, 1,
  4, 1, 0, 0, 0, 0, //35
  4, 0, 0, 0, 1, 0,
  4, 0, 1, 0, 0, 0,
  4, 0, 1, 0, 0, 0,
  4, 0, 0, 0, 1, 0,
  4, 0, 0, 1, 0, 0, //40-41
  8, 0, 0, 1, 0, 0,
  4, 0, 1, 0, 1, 0,
  4, 1, 0, 0, 0, 1,
  4, 1, 0, 0, 0, 1,
  16, 0, 0, 1, 0, 0, //45-49
  // long*8
  8, 0, 1, 0, 0, 1,
  8, 0, 0, 1, 0, 0,
  8, 1, 0, 0, 1, 0,
  8, 0, 0, 1, 0, 0,
  8, 0, 1, 0, 0, 1, //50
  8, 0, 0, 1, 0, 0,
  8, 1, 0, 0, 1, 0,
  8, 0, 0, 1, 0, 0,
  8, 0, 1, 0, 0, 1,
  8, 0, 0, 1, 0, 0, //55
  8, 1, 0, 0, 1, 0,
  8, 0, 0, 1, 0, 0,
  8, 0, 1, 0, 0, 1,
  8, 0, 0, 1, 0, 0,
  8, 1, 0, 0, 1, 0, //60
  8, 0, 0, 1, 0, 0,
  // short*5
  8, 0, 0, 1, 0, 1,
  8, 0, 0, 0, 1, 0,
  8, 1, 0, 1, 0, 0,
  8, 0, 1, 0, 0, 0, //65
  8, 0, 0, 1, 0, 1,
  8, 0, 0, 0, 1, 0,
  8, 1, 0, 1, 0, 0,
  8, 0, 1, 0, 0, 0,
  8, 0, 0, 1, 0, 1, //70
  8, 0, 0, 0, 1, 0,
  //
  4, 1, 0, 0, 0, 1,
  2, 0, 0, 1, 0, 0,
  4, 0, 0, 1, 0, 0,
  2, 0, 0, 1, 0, 0, //75
  2, 0, 1, 0, 1, 0,
  16, 0, 4, 0, 5, 0,

  // 1절
  16, 1, 0, 0, 0, 0,
  2, 1,0,0,0,0,
  2, 1,0,0,0,0, //80
  2, 0,0,0,0,1,
  4, 0,0,0,1,0,
  4, 0,0,0,1,0,
  4, 0,1,0,0,0,
  4, 1,0,0,0,0, //85
  4, 0,0,0,1,0,
  4, 0,0,1,0,0,
  4, 0,1,0,1,0,
  2, 0,0,2,0,0,
  4, 0,0,3,0,0, //90-109
  2, 0,0,0,0,1,
  4, 0,0,0,1,0,
  4, 0,1,0,0,0,
  4, 2,0,0,0,0,
  8, 3,0,0,0,0, //95
  16,0,1,0,0,0,
  4, 0,1,0,0,0,
  4, 0,0,0,0,1,
  4, 0,0,0,1,0,
  4, 0,0,0,1,0, //100
  4, 0,1,0,0,0,
  2, 0,1,0,0,0,
  2, 1,0,0,0,0,
  4, 0,0,0,1,0,
  8, 0,0,0,2,0, //105-127
  8, 0,0,0,3,0,
  4, 1,0,0,0,0,
  8, 0,1,0,0,0,
  4, 0,0,0,1,0,
  8, 0,0,0,0,1, //110
  4, 0,2,0,0,0,
  8, 0,3,0,0,0,
  
  /*
    30, 0, 0, 0, 1, 0, //0
    30, 0, 1, 0, 0, 0, //1
    30, 0, 0, 1, 0, 0, //2
    60, 0, 0, 2, 0, 0, //3
    30, 0, 0, 3, 0, 0, //4
    30, 0, 0, 0, 1, 0, //5
    60, 0, 1, 0, 0, 0, //6
    60, 0, 0, 0, 1, 0, //7
    60, 0, 1, 0, 0, 0, //8
    30, 0, 1, 0, 0, 0, //9
    30, 0, 0, 0, 1, 0, //10
    30, 0, 0, 0, 1, 0, //11
    30, 0, 0, 2, 0, 0, //12
    30, 0, 0, 3, 0, 0, //13
    30, 1, 0, 0, 0, 0, //14
    30, 0, 0, 0, 0, 1, //15
    30, 0, 0, 1, 0, 0, //16
    15, 0, 0, 0, 0, 1, //17
    15, 1, 0, 0, 0, 0, //18
    30, 0, 0, 0, 1, 0, //19
    15, 0, 1, 0, 0, 0, //20
    15, 0, 0, 0, 1, 0, //21
    30, 1, 0, 0, 0, 0, //22
    15, 0, 0, 0, 0, 1, //23
    15, 0, 1, 0, 0, 0, //24
    30, 0, 0, 1, 0, 0, //25
    30, 0, 0, 0, 1, 0, //26
    15, 0, 1, 0, 0, 0, //27
    15, 0, 0, 2, 0, 0, //28
    30, 0, 0, 3, 0, 0, //29
    30, 0, 0, 0, 1, 0, //30
    60, 0, 0, 1, 0, 0, //31
    30, 0, 0, 1, 0, 0, //32
    30, 0, 0, 1, 0, 0, //33
    30, 0, 1, 0, 0, 0, //34
    15, 0, 0, 0, 1, 0, //35
    15, 0, 1, 0, 1, 0, //36
    30, 1, 0, 0, 0, 1, //37
    30, 0, 0, 1, 0, 0, //38
    30, 0, 0, 0, 0, 1, //39
    15, 0, 0, 0, 0, 1, //40
    15, 0, 0, 0, 0, 1, //41
    30, 0, 1, 0, 0, 0, //42
    13, 0, 0, 1, 0, 0, //43
  */
  -1
};

////////////////////////////////////////////////
// state machine control
enum state_finger {
  SF_IDLE = 0,
  SF_TOUCH_MOVE_DOWN, SF_TOUCH_DOWN, SF_TOUCH_MOVE_UP,
  SF_LONG_MOVE_DOWN, SF_LONG_DOWN, SF_LONG_MOVE_UP,
  SF_LEFT_MOVE_DOWN, SF_LEFT_MOVE, SF_LEFT_BACK,
  SF_RIGHT_MOVE_DOWN, SF_RIGHT_MOVE, SF_RIGHT_BACK,
  SF_MAX
};

int state_delay_tick[ SF_MAX ] = {  // delay time for action
  0,
  10, 100, 10,
  10,   0, 10,     // 0 = infinite delay for long pattern
  10, 100, 10,
  10, 100, 10
};

state_finger  sf[ SERVO_COUNT ];  // state_finger
int   pos = 0;
long  sf_tick[ SERVO_COUNT ];     // wait until
unsigned long  start_tick = 0, last_tick = 0, play_tick = 0, step_tick = 0;

void state_change( int f, state_finger s, int ud = ANGLE_UD_UP, int lr = ANGLE_LR_CENTER )
{
  svo_ud[ f ].write( mid_value_ud[ f ] + ud );
  svo_lr[ f ].write( mid_value_lr[ f ] + lr );

  sf[ f ] = s;
  int delay_tick = state_delay_tick[ s ];
  sf_tick[ f ] = ( delay_tick > 0 ) ? ( play_tick + delay_tick ) : 0;

  sprintf( dbgmsg, "[%06lu] --change f=%d, state=%d, ud=%d, lr=%d, d=%d, w=%lu\n", play_tick, f, s, ud, lr, delay_tick, sf_tick[f] );
  DBG( dbgmsg );
}

////////////////////////////////////////////////
// finger control
void finger_init( int f )
{
  sprintf( dbgmsg, "-init f=%d\n", f );
  DBG( dbgmsg );

  state_change( f, SF_IDLE );
}

void finger_action( int f, int p )
{
  switch ( p ) {
    case 0 : // no change
      break;

    case 1 : // touch sequence start
      state_change( f, SF_TOUCH_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%06lu] TOUC %d @ %d\n", play_tick / spd, f, pos / 6 );
      break;

    case 2 : // long-note sequence start
      state_change( f, SF_LONG_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%06lu] DOWN %d @ %d\n", play_tick / spd, f, pos / 6 );
      break;

    case 3 : // long_note finish action
      state_change( f, SF_LONG_MOVE_UP, ANGLE_UD_UP );
      sprintf( dbgmsg, "[%06lu] UP   %d @ %d\n", play_tick / spd, f, pos / 6 );
      break;

    case 4 : // left sequence start
      state_change( f, SF_LEFT_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%06lu] LEFT %d @ %d\n", play_tick / spd, f, pos / 6 );
      break;

    case 5 : // right sequence start
      state_change( f, SF_RIGHT_MOVE_DOWN, ANGLE_UD_DOWN );
      sprintf( dbgmsg, "[%06lu] RIGHT %d @ %d\n", play_tick / spd, f, pos / 6 );
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
    case SF_TOUCH_MOVE_DOWN:    state_change( f, SF_TOUCH_DOWN, ANGLE_UD_DOWN );    break;
    case SF_TOUCH_DOWN:         state_change( f, SF_TOUCH_MOVE_UP, ANGLE_UD_UP );   break;
    case SF_TOUCH_MOVE_UP:      state_change( f, SF_IDLE );                         break;

    case SF_LONG_MOVE_DOWN:     state_change( f, SF_LONG_DOWN, ANGLE_UD_DOWN );     break;
    case SF_LONG_DOWN:                                                              break;    // infinite loop until up
    case SF_LONG_MOVE_UP:       state_change( f, SF_IDLE );                         break;

    case SF_LEFT_MOVE_DOWN:     state_change( f, SF_LEFT_MOVE, ANGLE_UD_DOWN, ANGLE_LR_LEFT );    break;
    case SF_LEFT_MOVE:          state_change( f, SF_LEFT_BACK );    break;
    case SF_LEFT_BACK:          state_change( f, SF_IDLE );         break;

    case SF_RIGHT_MOVE_DOWN:    state_change( f, SF_RIGHT_MOVE, ANGLE_UD_DOWN, ANGLE_LR_RIGHT );  break;
    case SF_RIGHT_MOVE:         state_change( f, SF_RIGHT_BACK );   break;
    case SF_RIGHT_BACK:         state_change( f, SF_IDLE );         break;
  }
}

////////////////////////////////////////////////
void setup()
{
  // serial debugger
  Serial.begin( SERIAL_SPEED );
  while ( !Serial )
  {
    ; // wait for serial connection
  }

  pinMode( START_PIN, INPUT );
  pinMode( LED_PIN, OUTPUT );
  for ( int i = 0; i < SERVO_COUNT; i ++ )
    svo_lr[i].attach( SERVO_PIN - i * 2 );
  for ( int i = 0; i < SERVO_COUNT; i ++ )
    svo_ud[i].attach( SERVO_PIN - i * 2 - 1 );

  // wait for start button
  MSG( "Push Button to Reset Position\n" );
  digitalWrite( LED_PIN, HIGH );
  while ( digitalRead( START_PIN ) == LOW );

  // reset positions
  for ( int i = 0; i < SERVO_COUNT; i ++ )
  {
    finger_init( i );
  }
  delay (1000);

  // wait for start button
  MSG( "Push Button to Start\n" );
  digitalWrite( LED_PIN, LOW );
  while ( digitalRead( START_PIN ) == LOW );

  last_tick = start_tick = millis();
  play_tick = step_tick = 0;
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
  else return;    // same milli-sec, no action

  for ( int i = 0; i < SERVO_COUNT; i++ )
    finger_process( i );

  // get time / check end mark
  int wait_tick = p[pos];
  if ( wait_tick < 0 )
  {
    static bool f = false;
    if ( !f ) {
      DBG( "FINISHED\n" );
      f = true;
    }
    return;
  }
  if ( step_tick < wait_tick * spd )
    return;
  pos ++;
  step_tick = 0;

  // move to next step
  for ( int i = 0; i < SERVO_COUNT; i++, pos++ )
    finger_action( i, p[pos] );
}

