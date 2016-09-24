#version 120

// these are for the programmable pipeline system
//uniform mat4 modelViewProjectionMatrix;
//in vec4 position;

// the time value is passed into the shader by the OF app.
uniform float time;


void main()
{
    // the sine wave travels along the x-axis (across the screen),
    // so we use the x coordinate of each vertex for the calculation,
    // but we displace all the vertex along the y axis (up the screen)/
    vec4 position = ftransform();

    float displacementHeight = 1000.0;
    //float displacementX = sin(time + (position.y / 100.0)) * displacementHeight;

    float displacementY = sin(time + (position.x*position.x / 100.0)) * displacementHeight;

    vec4 modifiedPosition = position;
   // modifiedPosition.x += displacementX;

    modifiedPosition.y += displacementY;
        gl_Position = modifiedPosition;
      //  gl_Position = ftransform();//modelViewProjectionMatrix * position;
    //    gl_Color =vec4(0.,0.,1.,1.);
}
