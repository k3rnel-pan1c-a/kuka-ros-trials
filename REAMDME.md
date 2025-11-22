## Documenting errors and their solutions...

1) Clock synchronization problem causing a problem when fetching the current pose for the robot through the move group interface
Solved through the running of the move group interface in a seperate executor alongside the main node which has it's own executor. 


