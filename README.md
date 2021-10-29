# mem_report_module

Kernel module that logs all the the PFNs of all the physical pages accessed by the kernel over a period of time. 

## Loading and Running the Module

1.  
```make run```
2. 
```insmod mem_report.ko```
3. 
```./mem_report <USER_PID>```
4. After previous script terminates
```./data_analyzer```




