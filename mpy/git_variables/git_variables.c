#include <yed/plugin.h>
#include <stdio.h>



/*COMMANDS */
void git_variables_post_pump(yed_event *event);
/*END COMMANDS*/


static unsigned long long last_update_time;

int yed_plugin_boot(yed_plugin *self) {
    char * output;
    int output_len, status;
    yed_event_handler h;

    YED_PLUG_VERSION_CHECK();

    h.kind = EVENT_POST_PUMP;
    h.fn   = git_variables_post_pump;
    yed_plugin_add_event_handler(self, h);


    yed_set_var("git-variables-branch", "");
    last_update_time=0;

    if(yed_get_var("git-variables-update-interval")==NULL){
        yed_set_var("git-variables-update-interval","1000");
    }
    if(yed_get_var("git-variables-branch-icon")==NULL){
        yed_set_var("git-variables-branch-icon","");
    }

    output = yed_run_subproc("basename `git rev-parse --show-toplevel`",&output_len,&status);
    if(status == 0&&output!=NULL){
        yed_set_var("git-variables-repo-name", output);
    }else{
        yed_set_var("git-variables-repo-name", "");
    }
    if(output!=NULL){
        free(output);
    }
    return 0;
}

void git_variables_post_pump(yed_event *event){
    unsigned long long current_time;
    char *output=NULL;
    int status;
    int output_len;
    unsigned long long update_interval;
    char* update_interval_string;
    char* buffer;
    char *icon_buffer;
    update_interval_string=yed_get_var("git-variables-update-interval");
    if(update_interval_string==NULL||sscanf(yed_get_var("git-variables-update-interval"),"%llu",&update_interval)!=1){
        update_interval=1000;
    }
    current_time = measure_time_now_ms();
    if((current_time-last_update_time)>update_interval){
        output = yed_run_subproc("git rev-parse --abbrev-ref HEAD",&output_len,&status);
        if(status != 0||output==NULL){
            yed_set_var("git-variables-branch", "");
        }
        else{
            icon_buffer = yed_get_var("git-variables-branch-icon");
            buffer = malloc(snprintf(NULL,0,"%s%s",icon_buffer==NULL?"":icon_buffer,output)+1);
            sprintf(buffer,"%s%s",icon_buffer==NULL?"":icon_buffer,output);
            yed_set_var("git-variables-branch", buffer);
            free(buffer);
        }
        if(output!=NULL){
            free(output);
        }
        last_update_time=measure_time_now_ms();
    }
}
