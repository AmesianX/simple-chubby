#!/usr/bin/python
import matplotlib.pyplot as plt

def DrawArrowFront(start_time, duration, start_event, end_event):
    plt.arrow(left_line,top_line-start_time,line_delta,-duration,
              head_width=0.1,head_length=0.1,length_includes_head=True,color='k')
    if start_event:
        plt.annotate(start_event,
                     xy=(left_line,top_line-start_time),
                     xytext=(left_line-text_width,top_line-start_time),
                     horizontalalignment='left',
                     arrowprops=dict(arrowstyle="-",color="k",linestyle='dashed'))
    if end_event:
        plt.annotate(end_event,
                     xy=(right_line,top_line-start_time-duration),
                     xytext=(right_line+text_width,top_line-start_time-duration),
                     horizontalalignment='right',
                     arrowprops=dict(arrowstyle="-",color="k",linestyle='dashed'))

def DrawArrowBack(start_time, duration, start_event, end_event):
    plt.arrow(right_line, top_line-start_time,-line_delta,-duration,
              head_width=0.1,head_length=0.1,length_includes_head=True,color='k')
    if start_event:
        plt.annotate(start_event,
                     xy=(right_line,top_line-start_time),
                     xytext=(right_line+text_width,top_line-start_time),
                     horizontalalignment='right',
                     arrowprops=dict(arrowstyle="-",color="k",linestyle='dashed'))
    if end_event:
        plt.annotate(end_event,
                     xy=(left_line,top_line-start_time-duration),
                     xytext=(left_line-text_width,top_line-start_time-duration),
                     horizontalalignment='left',
                     arrowprops=dict(arrowstyle="-",color="k",linestyle='dashed'))


left_line = 0
right_line = 0.5
line_delta = 0.5
bottom_line = 0
top_line = 10
text_width = 0.8
left_right_margin = 0.5
plt.plot([left_line]*2,[bottom_line,top_line],'k',linewidth=6)
plt.plot([right_line]*2,[bottom_line,top_line],'k',linewidth=6)

DrawArrowFront(1, 0.1, 'event send', 'event recv')
DrawArrowBack(1.5, 0.1, 'reply send', 'reply recv')

plt.axis('off')
plt.xlim(left_line-text_width-left_right_margin,
         right_line+text_width+left_right_margin)
plt.ylim(bottom_line, top_line)
plt.show();
