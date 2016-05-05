// Fingers v0.0
//    by cloudree@naver.com
//  v0.0 start for Dereste

// platform is Arduino MEGA

#include <Servo.h>
#include <String.h>

////////////////////////////////////////////////
const int FINGERS = 5;
const int PIN_START = 2;    // @ +( 0=start, 10-9, 8-7, 6-5, 4-3, 2-1 )

const int ANGLE_UD_UP = 0;
const int ANGLE_UD_DOWN = 10;

const int ANGLE_LR_LEFT = 10;
const int ANGLE_LR_CENTER = 20;
const int ANGLE_LR_RIGHT = 30;

Servo svo_ud[FINGERS];    // up-down
Servo svo_lr[FINGERS];    // left-right

long start_tick = 0;
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

int spd = 100;   // pattern speed (ms)
int p[P_MAX] = {
// onegai cindelela debut
 0, 0,0,0,0,0,
10, 1,0,0,0,0,
20, 0,1,0,0,0,
30, 0,0,1,0,0,
40, 0,0,0,1,0,
50, 0,0,0,0,1,
-1,             // TEST

 0, 0,0,0,1,0,  //24
30, 0,1,0,0,0,  //27
30, 0,0,1,0,0,  //30
50, 0,0,2,0,0,  //35
30, 0,0,3,0,0,  //38
30, 0,0,0,1,0,  //41
50, 0,1,0,0,0,  //46
60, 0,0,0,1,0,  //52
50, 0,1,0,0,0,  //57
30, 0,1,0,0,0,  //100
30, 0,0,0,1,0,  //103
30, 0,0,0,1,0,  //106
20, 0,0,2,0,0,  //108
30, 0,0,3,0,0,  //111
30, 1,0,0,0,0,  //114
20, 0,0,0,0,1,  //116
30, 0,0,1,0,0,  //119
20, 0,0,0,0,1,  //121
10, 1,0,0,0,0,  //122
30, 0,0,0,1,0,  //125
10, 0,1,0,0,0,  //126
10, 0,0,0,1,0,  //127
30, 1,0,0,0,0,  //130
10, 0,0,0,0,1,  //131
20, 0,1,0,0,0,  //133
30, 0,0,1,0,0,  //136
20, 0,0,0,1,0,  //138
20, 0,1,0,0,0,  //140
10, 0,0,2,0,0,  //141
30, 0,0,3,0,0,  //144
30, 0,0,0,1,0,  //147
50, 0,0,1,0,0,  //152
30, 0,0,1,0,0,  //155
30, 0,0,1,0,0,  //158
20, 0,1,0,0,0,  //200
20, 0,0,0,1,0,  //202
10, 0,1,0,1,0,  //203
30, 1,0,0,0,1,  //206
30, 0,0,1,0,0,  //209
20, 0,0,0,0,1,  //211
20, 0,0,0,0,1,  //213
10, 0,0,0,0,1,  //214
30, 0,1,0,0,0,  //217
10, 0,0,1,0,0,  //218
-1
};

////////////////////////////////////////////////
// debug : output to serial monitor
#define SERIAL_DEBUG
#define SERIAL_SPEED    115200

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
  50, 100, 50,
  50, -1, 50,     // -1 = as pattern
  50, 50, 50,
  50, 50, 50,
};

state_finger  sf[ FINGERS ];        // state_finger
long          sf_tick[ FINGERS ];   // wait until

void state_change( int f, state_finger s, int ud = -1, int lr = -1 ) {
  sf[ f ] = s;
  if( ud >= 0 ) svo_ud[ f ].write( ud );
  if( lr >= 0 ) svo_lr[ f ].write( lr );
  int delay_tick = state_delay_tick[ s ];  
  sf_tick[ f ] = ( delay_tick > 0 ) ? ( millis() + delay_tick ) : -1;
}

void DBG( int n ) {
  #ifdef SERIAL_DEBUG
    Serial.print( n );
  #endif
}

void DBG( char *msg ) {
  #ifdef SERIAL_DEBUG
    Serial.print( msg );
  #endif
}

////////////////////////////////////////////////
// finger control
void finger_init( int f ) {
  char msg[1000];
  sprintf( msg, "init %d\n", f );
  DBG( msg );
    
  svo_ud[ f ].write( ANGLE_UD_UP );
  svo_lr[ f ].write( ANGLE_LR_CENTER );
  sf[ f ] = SF_IDLE;
  sf_tick[ f ] = -1;
}

void finger_action( int f, int p ) {
  DBG( f );
  DBG( p );
  DBG( " " );

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
  int tm = sf_tick[ f ];
  if( tm < 0 )
    return;
  
  long cur = millis();
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
    while( !Serial ) 
    {
      ; // wait for serial connection
    }
  #endif
  DBG( "PUSH BUTTON TO START\n" );
  
  // pin setups
  pinMode( PIN_START, INPUT );
  for( int i=0; i<FINGERS; i++ ) {
    svo_ud[i].attach( PIN_START+10-i*2 );      // @+10,8,6,4,2
    svo_lr[i].attach( PIN_START+ 9-i*2 );      // @+ 9,7,5,3,1
  }

  // start position
  for( int i=0; i<FINGERS; i++ ) {
    //finger_init( i );
  }

  // wait for start
  while( digitalRead( PIN_START ) == LOW ) {
    delay( 100 );
    DBG( "," );
  }

  DBG( "STARTING\n" );
  start_tick = millis();
}

////////////////////////////////////////////////
void loop() {
  DBG( "," );
  int cur = millis() - start_tick;

  int ps = p[pos];
  for( int i=0; i<FINGERS; i++ )
    finger_process( i );

  // check end mark
  if( ps < 0 ) {
    //DBG( "FINISHED\n" );
    //pos = 0;     // restart
    return;
  }
  if( cur < ps * spd )
    return;

  // move to next step
  for( int i=0; i<FINGERS; i++ )
    finger_action( i, p[pos+1+i] );
  pos += FINGERS + 1;
  DBG( "\n" ); 
}


