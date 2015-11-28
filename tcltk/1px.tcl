#!/bin/sh
# the next line restarts using wish \
exec wish "$0" "$@"

wm title . "1px - Tcl/Tk version"
bind . <Escape> {exit}

canvas .can -width 200 -height 200 -background #000000
pack .can

set frame_count 0

proc draw {} {
    set c [expr {$::frame_count % 4}]
    
    if {$c == 0} {
        .can create line 100 100 101 101 -fill #ff0000
    } elseif {$c == 1} {
        .can create line 100 100 101 101 -fill #00ff00
    } elseif {$c == 2} {
        .can create line 100 100 101 101 -fill #0000ff
    } elseif {$c == 3} {
        .can create line 100 100 101 101 -fill #000000
    }
    
    puts $::frame_count
    incr ::frame_count 1
    
    after 250 draw
}

draw
