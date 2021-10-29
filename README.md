# mem_report_module

Kernel module that logs all the the PFNs of all the physical pages accessed by the kernel over a period of time. 

## Loading and Running the Module

1.  
```bash
make run
```
2. 
```bash
insmod mem_report.ko
```
3. 
```bash
./mem_report <USER_PID>
```
4. After previous script terminates
```bash
./data_analyzer
```




