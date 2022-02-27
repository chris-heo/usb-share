pcb_size = [50, 40, 1.6];
pcb_compheight_bot = 2.5;
pcb_compheight_top = 10.5;

box_bottom_thickness = 1;
box_top_thickness = 1;
box_wall_thickness = 1.2;

screwhead_diam = 3.7;


pcb_clearance_left = 1.25;
pcb_clearance_front = 1.25 + 2;
pcb_clearance_right = 1.25;
pcb_clearance_back = 1.25;

pcb_holes = [
    //x, y, diameter
    [2.5, 8.925, 2.3],
    [47.5, 8.925, 2.3],
    [2.5, 37.5, 2.3],
    [47.5, 37.5, 2.3]
];

led_holes = [
//plunge diam = 3.5
    [16.66875, 2, 4],
    [33.3375, 2, 4]
];

button_holes = [
    [10, 5.5, 4],
    [40, 5.5, 4]
];


overprint = 0.15;
studdiam = 5;

box_size = pcb_size 
    + [pcb_clearance_left + pcb_clearance_right, pcb_clearance_front + pcb_clearance_back, 0] // pcb clearance
    + [box_wall_thickness * 2, box_wall_thickness * 2, box_bottom_thickness] // add thicknesses to outline
    + [0, 0, pcb_compheight_bot + pcb_compheight_top]; // component height


echo("box size:", box_size);

box_offset = [ 
    (pcb_clearance_right - pcb_clearance_left) / 2,
    (pcb_clearance_back - pcb_clearance_front) / 2,
    box_size.z / 2 - box_bottom_thickness
];