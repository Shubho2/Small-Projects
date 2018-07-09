# Import a library of functions called 'pygame'
import pygame
from math import pi
 
# Initialize the game engine
# pygame.init()
 
# Define the colors we will use in RGB format
BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
BLUE =  (  0,   0, 255)
GREEN = (  0, 255,   0)
RED =   (255,   0,   0)
 
# Set the height and width of the screen
size = [400, 300]
screen = pygame.display.set_mode(size)
 
pygame.display.set_caption("Example code for the draw module")
 
#Loop until the user clicks the close button.
done = False
clock = pygame.time.Clock()
 
while not done:
 
    # This limits the while loop to a max of 10 times per second.
    # Leave this out and we will use all CPU we can.
    clock.tick(10)
     
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
 
    # All drawing code happens after the for loop and but
    # inside the main while done==False loop.
     
    # Clear the screen and set the screen background
    screen.fill(WHITE)
 
    # Draw on the screen a GREEN line from (0,0) to (50.75) 
    # 5 pixels wide.

    # Draw a rectangle outline
    pygame.draw.rect(screen, BLACK, [150, 10, 20, 20])
    # Draw a solid rectangle
    pygame.draw.rect(screen, BLACK, [150, 110, 20, 20])
    # Draw a rectangle outline
    pygame.draw.rect(screen, BLACK, [75, 50, 20, 20])
    # Draw a solid rectangle
    pygame.draw.rect(screen, BLACK, [225, 50, 20, 20])
     
    pygame.draw.line(screen, GREEN, [150, 10], [75,50], 5)
    
    # Draw a circle
    pygame.draw.circle(screen, BLUE, [150, 80], 5)
    pygame.font.Font.render("text", True, RED, background=None)
    
    # Go ahead and update the screen with what we've drawn.
    # This MUST happen after all the other drawing commands.
    pygame.display.flip()
 
# Be IDLE friendly
pygame.quit()