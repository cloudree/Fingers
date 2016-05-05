// Fingers v0.0
//    by cloudree@naver.com
//  v0.0 start for Dereste

// platform is Arduino MEGA

#include <Time.h>
#include <Servo.h>

////////////////////////////////////////////////
const int FINGERS = 5;
const int PIN_START = 0;    // 0=start, 1-2, 3-4, 5-6, 7-8, 9-10 (PWM)

const int ANGLE_UD_UP = 0;
const int ANGLE_UD_DOWN = 10;

const int ANGLE_LR_LEFT = 10;
const int ANGLE_LR_CENTER = 20;
const int ANGLE_LR_RIGHT = 30;

Servo svo_ud[FINGERS];    // up-down
Servo svo_lr[FINGERS];    // left-right

time_t start_tick = 0;
int pos = 0;

////////////////////////////////////////////////
// pattern
//   time, F1, F2, F3 ... 

//   time is
//    -1 = end

//   Fx is state such as
//    0 = no change
//    1 = touch (down & up)
//    2 = down for long note
//    3 = up for long note
//    4 = left shift
//    5 = right shift

const int P_MAX = 500 * (FINGERS + 1);

int spd = 500;   // pattern speed
int p[P_MAX] = {
  0, 0, 0, 0, 0, 0,
  1, 1, 0, 0, 0, 0,
  2, 0, 1, 0, 0, 0,
  3, 0, 0, 1, 0, 0,
  4, 0, 0, 0, 1, 0,
  5, 0, 0, 0, 0, 1,
  -1
};

////////////////////////////////////////////////
// debug : output to serial monitor
#define SERIAL_DEBUG
#define SERIAL_SPEED    19200

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

int state_delay[ SF_MAX ] = {  // delay time for action
  0,
  50, 100, 50,
  50, -1, 50,     // -1 = as pattern
  50, 50, 50,
  50, 50, 50,
};

state_finger  sf[ FINGERS ];        // state_finger
time_t       sf_time[ FINGERS ];   // wait until

void state_change( int f, state_finger s, int ud = -1, int lr = -1 ) {
  sf[ f ] = s;
  if( ud >= 0 ) svo_ud[ f ].write( ud );
  if( lr >= 0 ) svo_lr[ f ].write( lr );
  int delay_time = state_delay[ s ];  
  sf_time[ f ] = ( delay_time > 0 ) ? ( now() + delay_time ) : -1;
}

////////////////////////////////////////////////
// finger control
void finger_init( int f ) {
  svo_ud[ f ].write( ANGLE_UD_UP );
  svo_lr[ f ].write( ANGLE_LR_CENTER );
  sf[ f ] = SF_IDLE;
  sf_time[ f ] = -1;
}

void finger_action( int f, int p ) {
  #ifdef SERIAL_DEBUG
    Serial.println( f, p );
  #endif

  switch( p ) {
  case 0 : // no change
    break;
    
  case 1 : // touch sequence start 
    state_change( f, SF_TOUCH_MOVE_DOWN, ANGLE_UD_DOWN );  
    break;
    
  case 2 : // long-note sequence start
    state_change( f, SF_LONG_MOVE_DOWN, ANGLE_UD_DOWN );
    break;
    
  case 3 : // long_note finish action
    state_change( f, SF_LONG_MOVE_UP, ANGLE_UD_UP );
    break;
    
  case 4 : // left sequence start
    state_change( f, SF_LEFT_MOVE_DOWN, ANGLE_UD_DOWN );
    break;
    
  case 5 : // right sequence start
    state_change( f, SF_RIGHT_MOVE_DOWN, ANGLE_UD_DOWN );
    break;
  }
}

void finger_process( int f ) {
  int tm = sf_time[ f ];
  if( tm < 0 )
    return;
  
  int cur = now();
  if( cur < tm )
    return;

  switch( sf[f] ) {
  case SF_IDLE:
    break;

  // change to next state ...
  case SF_TOUCH_MOVE_DOWN:    state_change( f, SF_TOUCH_DOWN );                   break;
  case SF_TOUCH_DOWN:         state_change( f, SF_TOUCH_MOVE_UP, ANGLE_UD_UP );   break;
  case SF_TOUCH_MOVE_UP:      state_change( f, SF_IDLE );                         break;
  
  case SF_LONG_MOVE_DOWN:     state_change( f, SF_LONG_DOWN );                    break;
  case SF_LONG_DOWN:                                                              break;
  case SF_LONG_MOVE_UP:       state_change( f, SF_IDLE );                         break;
  
  case SF_LEFT_MOVE_DOWN:     state_change( f, SF_LEFT_MOVE, -1, ANGLE_LR_LEFT );              break;
  case SF_LEFT_MOVE:          state_change( f, SF_LEFT_BACK, ANGLE_UD_UP, ANGLE_LR_CENTER );   break;
  case SF_LEFT_BACK:          state_change( f, SF_IDLE );                                      break;
  
  case SF_RIGHT_MOVE_DOWN:    state_change( f, SF_RIGHT_MOVE, -1, ANGLE_LR_RIGHT );            break;
  case SF_RIGHT_MOVE:         state_change( f, SF_RIGHT_BACK, ANGLE_UD_UP, ANGLE_LR_CENTER );  break;
  case SF_RIGHT_BACK:         state_change( f, SF_IDLE );                                      break;
  }
}

////////////////////////////////////////////////
void setup() {

  // serial debugger
  #ifdef SERIAL_DEBUG
    Serial.begin( SERIAL_SPEED );
    while( !Serial ) {
      ; // wait for serial connection
    }
  #endif
  
  // pin setups
  pinMode( PIN_START, INPUT );
  for( int i=0; i<FINGERS; i++ ) {
    svo_ud[i].attach( PIN_START+1+i*2 );      // 1,3,5,7,9
    svo_lr[i].attach( PIN_START+2+i*2 );      // 2,4,6,8,10
  }

  // start position
  for( int i=0; i<FINGERS; i++ ) {
    finger_init( i );
  }

  // wait for start
  while( digitalRead( PIN_START ) == LOW ) {
    delay( 100 );
  }
  start_tick = now();
}

////////////////////////////////////////////////
void loop() {
  int cur = now() - start_tick;

  while(1) {
    int ps = p[pos];
    for( int i=0; i<FINGERS; i++ )
      finger_process( i );

    // check end mark
    if( ps < 0 ) {
      //pos = 0;     // restart
      break;
    }
    if( cur < ps * spd )
      break;

    // move to next step
    for( int i=0; i<FINGERS; i++ )
      finger_action( i, p[pos+1+i] );
    pos += FINGERS + 1;
    
    // 
    break;
  }  
}

