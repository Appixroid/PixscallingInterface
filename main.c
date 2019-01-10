#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#define FILTER_COUNT 5
/** Filters Data **/
const char* filters[FILTER_COUNT] = {"scale", "epx", "scale2x", "scale3x", "eagle"};

/** Settings **/
char* inputFile = NULL;
char* outputFile = NULL;
const char* filter = NULL;
int repetition = 1;
int speedTest = 0;
int memoryLeak = 0;


//MAIN WINDOW
GtkWidget* mainWindow;

/** Functions **/
char* str_replace(char *orig, char *rep, char *with);
void updateInputFile(GtkWidget* widget, gpointer data);
void viewInput(GtkWidget* widget, gpointer data);
void updateOutputFile(GtkWidget* widget, gpointer data);
void viewOutput(GtkWidget* widget, gpointer data);
void updateFilter(GtkWidget* widget, gpointer data);
void updateRepetition(GtkWidget* widget, gpointer data);
void updateSpeedTest(GtkWidget* widget, gpointer data);
void updateMemoryLeak(GtkWidget* widget, gpointer data);
void upscale(GtkWidget* widget, gpointer data);
void popOpenImage(const char* imagePath);

int main(int argc,char **argv)
{
	int inputSet = 0;
	int outputSet = 0;
	int filterSet = 0;

	/***** Element *****/
    GtkWidget* mainLayout;
    
    GtkWidget* labelLayout;
    GtkWidget* titleLabel;
    GtkWidget* subtitleLabel;
    
    GtkWidget* inputLayout;
    GtkWidget* inputLabel;
    GtkWidget* inputChooser;
    GtkWidget* inputViewer;
    
    GtkWidget* outputLayout;
    GtkWidget* outputLabel;
    GtkWidget* outputChooser;
    GtkWidget* outputViewer;
    
    GtkWidget* filterLayout;
    GtkWidget* filterLabel;
    GtkWidget* filterBox;
    
    GtkWidget* scaleLayout;
    GtkWidget* scaleLabel;
    GtkWidget* scaleButton;
    
    GtkWidget* debugLayout;
    GtkWidget* debugLabel;
    GtkWidget* speedTestCheck;
    GtkWidget* memoryLeakCheck;
    
    GtkWidget* upscaleButton;
 
 	/***** INIT *****/
    gtk_init(&argc,&argv);
 
	 	/*** MAIN WINDOW ***/
		mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(mainWindow), "Pixscalling");
		gtk_window_set_position(GTK_WINDOW(mainWindow), GTK_WIN_POS_CENTER);
		gtk_window_set_default_size(GTK_WINDOW(mainWindow), 350, 300);
		
		/*** MAIN LAYOUT ***/
		mainLayout = gtk_vbox_new(TRUE, 0);
		gtk_container_add(GTK_CONTAINER(mainWindow), mainLayout);
		
		/*** LABELS ***/
		labelLayout = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(mainLayout), labelLayout, TRUE, FALSE, 0);
	 
			/* Label Title */
			titleLabel = gtk_label_new("<big><b>Pixscalling</b></big>");
			gtk_label_set_use_markup(GTK_LABEL(titleLabel), TRUE);
			gtk_box_pack_start(GTK_BOX(labelLayout), titleLabel, FALSE, FALSE, 0);
		
			/* Label Subtitle */
			subtitleLabel = gtk_label_new("<small><i>Apply Upscalling Filter on Pixel Art</i></small>");
			gtk_label_set_use_markup(GTK_LABEL(subtitleLabel), TRUE);
			gtk_box_pack_start(GTK_BOX(labelLayout), subtitleLabel, FALSE, FALSE, 0);
			
		/*** INPUT FILE ***/
		inputLayout = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(mainLayout), inputLayout, TRUE, FALSE, 0);
		
			/* Label  Input */
			inputLabel = gtk_label_new("Input File : ");
			gtk_box_pack_start(GTK_BOX(inputLayout), inputLabel, FALSE, FALSE, 20);
			
			/* Input Chooser */
			inputChooser = gtk_file_chooser_button_new("Choose Input File...", GTK_FILE_CHOOSER_ACTION_OPEN);
			gtk_box_pack_start(GTK_BOX(inputLayout), inputChooser, TRUE, TRUE, 0);
			
			/* Input Viewer */
			inputViewer = gtk_button_new_with_label("View");
			gtk_box_pack_start(GTK_BOX(inputLayout), inputViewer, FALSE, FALSE, 20);
			
		/*** OUTPUT FILE ***/
		outputLayout = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(mainLayout), outputLayout, TRUE, FALSE, 0);
		
			/* Label  Output */
			outputLabel = gtk_label_new("Output File : ");
			gtk_box_pack_start(GTK_BOX(outputLayout), outputLabel, FALSE, FALSE, 20);
			
			/* Output Chooser */
			outputChooser = gtk_file_chooser_button_new("Choose Output File...", GTK_FILE_CHOOSER_ACTION_OPEN);
			gtk_box_pack_start(GTK_BOX(outputLayout), outputChooser, TRUE, TRUE, 0);
			
			/* Output Viewer */
			outputViewer = gtk_button_new_with_label("View");
			gtk_box_pack_start(GTK_BOX(outputLayout), outputViewer, FALSE, FALSE, 20);
			
		/*** FILTER SELECT **/
		filterLayout = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(mainLayout), filterLayout, FALSE, FALSE, 0);
		
			/* Label Filter */
			filterLabel = gtk_label_new("Filter : ");
			gtk_box_pack_start(GTK_BOX(filterLayout), filterLabel, FALSE, FALSE, 20);
			
			/* Filter Combo Box */
			filterBox = gtk_combo_box_text_new();
			for(int i = 0; i < FILTER_COUNT; i++)
			{
				gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(filterBox), i, filters[i]);	
			}
			gtk_box_pack_start(GTK_BOX(filterLayout), filterBox, FALSE, FALSE, 20);
			
		/*** SCALE SELECT ***/
		scaleLayout = gtk_hbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(mainLayout), scaleLayout, FALSE, FALSE, 0);
			
			/* Scale Label */
			scaleLabel = gtk_label_new("Application Repetition : ");
			gtk_box_pack_start(GTK_BOX(scaleLayout), scaleLabel, FALSE, FALSE, 20);
			
			/* Scale Spin Button */
			scaleButton = gtk_spin_button_new(GTK_ADJUSTMENT(gtk_adjustment_new(1, 1, 100, 1, 0, 0)), 0.5, 0);
			gtk_box_pack_start(GTK_BOX(scaleLayout), scaleButton, FALSE, FALSE, 20);
			
		/*** DEBUG PART ***/
		debugLayout = gtk_vbox_new(FALSE, 0);
		gtk_box_pack_start(GTK_BOX(mainLayout), debugLayout, FALSE, FALSE, 0);
		
			/* Debug Label */
			debugLabel = gtk_label_new("Debug Option : ");
			gtk_box_pack_start(GTK_BOX(debugLayout), debugLabel, FALSE, FALSE, 0);
			
			/* Speed Test */
			speedTestCheck = gtk_check_button_new_with_label("Speed Test");
			gtk_box_pack_start(GTK_BOX(debugLayout), speedTestCheck, FALSE, FALSE, 0);
			
			/* Memory Leak */
			memoryLeakCheck = gtk_check_button_new_with_label("Memory Leak");
			gtk_box_pack_start(GTK_BOX(debugLayout), memoryLeakCheck, FALSE, FALSE, 0);
			
		/*** Scale Button ***/
		upscaleButton = gtk_button_new_with_label("Upscale !");
		gtk_box_pack_start(GTK_BOX(mainLayout), upscaleButton, TRUE, TRUE, 20);
	 
	/**** SHOW *****/
	gtk_widget_show_all(mainWindow);
	 
	/***** SIGNALS *****/
		/*** MAIN WINDOW **/
		g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
 		
 		/*** INPUT FILE ***/
 		g_signal_connect(G_OBJECT(inputChooser), "selection-changed", G_CALLBACK(updateInputFile), NULL);
 		
 		/*** INPUT VIEWER **/
 		g_signal_connect(G_OBJECT(inputViewer), "clicked", G_CALLBACK(viewInput), NULL);
 	
 		/*** OUTPUT FILE ***/
 		g_signal_connect(G_OBJECT(outputChooser), "selection-changed", G_CALLBACK(updateOutputFile), NULL);
 		
 		/*** OUTPUT VIEWER **/
 		g_signal_connect(G_OBJECT(outputViewer), "clicked", G_CALLBACK(viewOutput), NULL);
 		
 		/*** FILTER ***/
 		g_signal_connect(G_OBJECT(filterBox), "changed", G_CALLBACK(updateFilter), NULL);
 		
 		/*** REPETITION ***/
 		g_signal_connect(G_OBJECT(scaleButton), "value-changed", G_CALLBACK(updateRepetition), NULL);
 		
 		/*** SPEED TEST ***/
 		g_signal_connect(G_OBJECT(speedTestCheck), "toggled", G_CALLBACK(updateSpeedTest), NULL);
 		
 		/*** MEMORY LEAK ***/
 		g_signal_connect(G_OBJECT(memoryLeakCheck), "toggled", G_CALLBACK(updateMemoryLeak), NULL);
 		
 		/*** UPSCALE BUTTON ***/
 		g_signal_connect(G_OBJECT(upscaleButton), "clicked", G_CALLBACK(upscale), NULL);
 		
 	/***** START *****/
    gtk_main();
 
    return EXIT_SUCCESS;
}

char *str_replace(char *orig, char *rep, char *with)
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

void updateInputFile(GtkWidget* widget, gpointer data)
{
	free(inputFile);
	inputFile = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
}

void viewInput(GtkWidget* widget, gpointer data)
{
	popOpenImage(inputFile);
}

void updateOutputFile(GtkWidget* widget, gpointer data)
{
	free(outputFile);
	outputFile = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
}

void viewOutput(GtkWidget* widget, gpointer data)
{
	popOpenImage(outputFile);
}

void updateFilter(GtkWidget* widget, gpointer data)
{
	filter = filters[gtk_combo_box_get_active (GTK_COMBO_BOX(widget))];
}

void updateRepetition(GtkWidget* widget, gpointer data)
{
	repetition = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget));
}

void updateSpeedTest(GtkWidget* widget, gpointer data)
{
	speedTest = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void updateMemoryLeak(GtkWidget* widget, gpointer data)
{
	memoryLeak = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void upscale(GtkWidget* widget, gpointer data)
{
	if(inputFile != NULL && outputFile != NULL && filter != NULL && repetition >= 1)
	{
		char command[500];
	
		char* inputPath = str_replace(inputFile, " ", "\\ ");
		char* outputPath = str_replace(outputFile, " ", "\\ ");
	
		char options[3] = {'\0', '\0', '\0'};
	
		if(speedTest >= 1 && memoryLeak >= 1)
		{
			options[0] = '-';
			options[1] = 's';
			options[2] = 'm';
		}
		else if(speedTest >= 1)
		{
			options[0] = '-';
			options[1] = 's';
		}
		else if(memoryLeak >= 1)
		{
			options[0] = '-';
			options[1] = 'm';
		}
	
		sprintf(command, "./pixscalling %s \"%s\" \"%s\" %s %i", options, inputPath, outputPath, filter, repetition);
		system(command);
				
		free(inputPath);
		free(outputPath);
		
		popOpenImage(outputFile);
	}
	else
	{
		GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(mainWindow), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Upscale Uncompleted");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}
}

void popOpenImage(const char* imagePath)
{
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Image Preview");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		
 	GtkWidget* image = gtk_image_new_from_file(imagePath);
 	
	gtk_container_add(GTK_CONTAINER(window), image);
 	gtk_widget_show_all(window);
}
