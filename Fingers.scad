// Toucher

target = 2;
// 0 = inter motor
// 1 = finger 
// 2 = hand parm

// common
thick = 5;
thick2 = 3;

// for MG-90s
len = 23;
height = 12.5+1;
width = 19;

// inter-motor
inmot = 17 + 5;

// finger
flen = 100;
fdia = 8;
pendia = 10;
fixer = 6;

$fn = 16;

module InterMotor() {
    difference() {
        cube([ height+ thick, len+ thick*2, 10 ]);
        translate([ -1, thick, -1 ])
            cube([ height+1, len, 12 ]);
    }

    difference() {
        translate([ height,0,0 ])
            cube([ thick, len+ thick*2, 5+ inmot ]);
        translate([ height-1, 10, inmot ])
            rotate([ 0, 90, 0 ])
                cylinder( d=2, h=thick+2 );
    }

    translate([ 0, thick, 0 ])
        cylinder( d=2, h=10 );

    translate([ 0, thick+len, 0 ])
        cylinder( d=2, h=10 );
}

module FingerBody() {
    difference() {
        union() {
            cube([ thick, flen, fdia ]);
            translate([ thick/2, flen-(pendia+thick)/2-fixer, 0 ])
                cylinder( d=pendia+thick, h=fdia );
        }
        translate([ -1, 5, fdia/2 ])
            rotate([ 0, 90, 0 ])
                cylinder( d=2, h=thick+2 );
        translate([ -1, 5+8, fdia/2 ])
            rotate([ 0, 90, 0 ])
                cylinder( d=2, h=thick+2 );

        // pen fixer
        translate([ thick/2, flen-(pendia+thick)/2-fixer, 0 ])
            cylinder( d=pendia+1, h=fdia );
        translate([ -1, flen-fixer/2, fdia/2 ])
            rotate([ 0, 90, 0 ])
                cylinder( d=2, h=thick+2 );        
        translate([ thick/2-1, flen-thick-8, -1 ])
            cube([ 2, thick+9, fdia+2 ]);
    }
}

module HandParm() {
    difference() {
        cube([ len+thick2*2.5, height+thick2*2, width+thick2 ]);
        translate([ thick2-0.5, thick2, thick2 ])
            cube([ len+0.5, height-0.5, width+2 ]);
        translate([ -1, height/2+thick2/2, thick2 ])
            cube([ 10, thick2, width+1 ]);
        translate([ len+thick2*1.5+0.5, height/2+thick2, width-9 ])
            cylinder( d=2, h=20 );
    }
    difference() {
        translate([ -20, 0, 0 ])
            cube([ len+thick2*2+40, height+thick2*2, thick2 ]);
        translate([ -10, height*0.2+thick2, -1 ])
            cylinder( d=2, h=thick2+2 );
        translate([ -10, height*0.8+thick2, -1 ])
            cylinder( d=2, h=thick2+2 );
        translate([ len+thick2*2+10, height*0.2+thick2, -1 ])
            cylinder( d=2, h=thick2+2 );
        translate([ len+thick2*2+10, height*0.8+thick2, -1 ])
            cylinder( d=2, h=thick2+2 );
    }
}

if( target == 0 ) InterMotor();
if( target == 1 ) FingerBody();
if( target == 2 ) HandParm();

