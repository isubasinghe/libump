# libump
Current IPC in Linux is a bit slow as it goes via kernel space, this is an attempt to introduce User Level Message Passing employed in many research OSes such as ETH Zuerich's Barrelfish to Linux.

~~This is a TODO: I will implement this after I am unfucked by the advanced operating systems subject~~ 
YAY! I have been unfucked, this is now a WIP. 


HOW:
- shm_open 
- mmap 
- ringbuffer go brrrrr

I hope you enjoyed the thorough technical discussion above.
