sketch.default2d();
sketch.fsaa = 0; // disable fsaa

var color_red   = [1.0, 0.0, 0.0, 1.0];
var color_green = [0.0, 1.0, 0.0, 1.0];
var color_blue  = [0.0, 0.0, 1.0, 1.0];
var color_black = [0.0, 0.0, 0.0, 1.0];

var count = 0;

draw();

function draw()
{
	var width = box.rect[2] - box.rect[0];
	
	with (sketch) {
		// clear background
		glclearcolor([0.0, 0.0, 0.0, 1.0]);
		glclear();			

		moveto(0, 0); // -1.0 - 1.0

		// fill bgcircle
		switch(count % 4) {
			case 0:
				glcolor(color_red);
				break;
			case 1:
				glcolor(color_green);
				break;
			case 2:
				glcolor(color_blue);
				break;
			case 3:
				glcolor(color_black);
				break;
		}			
		point(0, 0);
		
		count ++;
	}
}

function bang()
{
	draw();
	refresh();
}

function onresize(w,h)
{
	// fix box.size()...
	box.size(200, 200);

	draw();
	refresh();
}
