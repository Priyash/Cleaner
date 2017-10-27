import time
import schedule
import subprocess

desktop_cleaner_path = "C:/Users/PRIYASH_11/Documents/Visual Studio 2015/Projects/Cleaner/Debug/Cleaner.exe"

def call_desktop_cleaner_exe():

	f = subprocess.call(desktop_cleaner_path)
	return
#SCHEDULE JOB
def schedule_job(start_time):

	schedule.every().day.at(start_time).do(call_desktop_cleaner_exe)
	print "Scheduled at " + start_time
	while 1:
		schedule.run_pending()
		time.sleep(1)
	return




def start_cleaner():
	schedule_job("16:16");
	return


if __name__ == '__main__':
	start_cleaner()
