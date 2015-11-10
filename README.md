# Suicide_Detect_Railway
OpenCV (C++) code that uses blob detection and background modelling to detect a possible suicide attempt

The algorithm works for well for open tracks. It involves the following steps:

-Create rectangular ROI at the track center
-Blur the ROI so as to neglect minor track jitter
-perform background subtraction
-enclose larger foreground contours in rects
-if rect area exceeds a threshold, and it is located between the tracks, putText()

The model and ROI have been calibrated according to the video available in the repo.
The shell file can be used on Linux systems with OpenCV 2.4+ and gpp.
