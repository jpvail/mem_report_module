#include <linux/delay.h> 
#include <asm/segment.h> 
#include <linux/buffer_head.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/pagewalk.h>
#include <linux/kallsyms.h>
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/thread_info.h> 
#include "mem_report.h"

//for writing to file
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fcntl.h>
#include <linux/uaccess.h>

static int mem_report_major; 
static struct class  *mem_report_class;
static struct file *file_struct; 
static unsigned long long pos = 0;

struct file *file_open(const char *path, int flags, int rights) 
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    int err = 0;
	
	
    oldfs = get_fs();
    set_fs(KERNEL_DS);
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

void file_close(struct file *file) 
{
    filp_close(file, NULL);
}

int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    ret = kernel_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(KERNEL_DS);

    ret = kernel_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}



int read_pte(pte_t *pte, unsigned long addr, unsigned long next, 
		struct mm_walk *walk)
{ 
	unsigned char buf[100]; 
	pte_t val = ptep_get(pte); 
	if(pte_present(val) && pte_young(val)){
		//printk("mem_report page accessed: %lu\n", ((pte_val(val) & PAGE_MASK) >> PAGE_SHIFT) );
		sprintf(buf, "%lu\n", ((pte_val(val) & PAGE_MASK) >> PAGE_SHIFT)); 
		file_write(file_struct, pos, buf, strlen(buf));   
		pos += strlen(buf);
	}

	return 0; 
}

int write_to_pte(pte_t *pte, unsigned long addr, unsigned long next,
			struct mm_walk *walk)
{
	pte_t val = pte_mkold( ptep_get(pte) ); 
	
	set_pte_atomic(pte, val);

	return 0; 
}

struct mm_walk_ops read_ops = {
	.pte_entry = read_pte,
}; 

struct mm_walk_ops write_ops = {
	.pte_entry = write_to_pte,
}; 

static int mem_report_open(struct inode *i, struct file *f)
{
	return 0;
}

static  int mem_report_release(struct inode *i, struct  file *f)
{
	return 0;
}

static long mem_report_ioctl(struct file *file, unsigned int cmd,
				unsigned long arg)
{

	int __user karg; 
	struct task_struct *task;
	struct pid *pid_struct;  
	struct mm_struct *pid_mm; 
	unsigned  long i; 
	unsigned long j; 	
	unsigned long k; 
	unsigned char *buf = "DONE\n"; 
	

	if (copy_from_user(&karg, (void *)arg, sizeof(int))) {
		printk("failed to copy_from_user()\n");
		return -EFAULT;
	}
	
	pid_struct = find_get_pid(karg); 
	
	rcu_read_lock(); 
        task = pid_task(pid_struct, PIDTYPE_PID);
        rcu_read_unlock();

	pid_mm = task->mm; 

	file_struct = file_open("/home/jason/Desktop/mem_data.txt", O_RDWR | O_CREAT, 0777); 
 
	switch(cmd){
		case MEM_REPORT_RUN:
			//for(k = 0; k < 20; k++){ 
				printk("mem_report: walk_page_range_novma started\n");

                        	for(i = 0x0; i < 0xffffffffffffffff; i += 0x0001000000000000){
                                	j = i + 0x0001000000000000; 
                                	walk_page_range_novma(pid_mm, i,/* 0xffffffffff600ff*/ j, 
                                                		&write_ops, pid_mm->pgd, NULL);
                        	}
				//msleep(5000); 
                       		for(i = 0x0; i < 0xffffffffffffffff; i += 0x0001000000000000){
                                	j = i + 0x0001000000000000;
                               		walk_page_range_novma(pid_mm, i,/* 0xffffffffff600ff*/ j, 
                                                		&read_ops, pid_mm->pgd, NULL); 
                        	} 
                        	file_write(file_struct, pos, buf, strlen(buf));
                        	pos += strlen(buf); 
                        	printk("mem_report: walk_page_range_novma finished\n"); 
			//} 
			break;

	}

	file_close(file_struct); 	

	return 0; 
}

static struct  file_operations mem_report_fops = {
	.open = mem_report_open,
	.release = mem_report_release,
	.unlocked_ioctl = mem_report_ioctl,
}; 

static int __init mem_report_init(void)
{

	mem_report_major = register_chrdev(0, "mem_report", &mem_report_fops);

	if (mem_report_major < 0) {
		printk("failed to register device\n");
		return mem_report_major; 
	}

	mem_report_class = class_create(THIS_MODULE, "mem_report");
	if (IS_ERR(mem_report_class)) {
		printk("failed to create class\n");
		unregister_chrdev(mem_report_major, "mem_report");
		return 1; 
	}

	device_create(mem_report_class, NULL, MKDEV(mem_report_major, 0), NULL,
		      "mem_report");

	printk("mem_report module loaded\n");
	
	return 0;

}

static void __exit mem_report_exit(void)
{
	device_destroy(mem_report_class, MKDEV(mem_report_major, 0));
	class_destroy(mem_report_class);
	unregister_chrdev(mem_report_major, "mem_report");

	printk("mem_report module unloaded\n");
}

module_init(mem_report_init);
module_exit(mem_report_exit);

MODULE_LICENSE("GPL");
