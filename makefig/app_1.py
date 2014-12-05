#!/usr/bin/python
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

def DrawArrowFront(start_time, duration, name, start_event, end_event):
    plt.arrow(left_line,top_line-start_time,line_delta,-duration,
              head_width=0.1,head_length=0.1,length_includes_head=True,color='k')
    if name:
        plt.annotate(name,
                     xy=(left_line+line_delta/2,
                         text_float+top_line-start_time-duration/2),
                     horizontalalignment='center')
    if start_event:
        plt.annotate(start_event,
                     xy=(left_line,top_line-start_time),
                     xytext=(left_line-text_width*len(start_event),top_line-start_time),
                     horizontalalignment='left',
                     arrowprops=dict(arrowstyle="->",color="k",linestyle='dashed'))
    if end_event:
        plt.annotate(end_event,
                     xy=(right_line,top_line-start_time-duration),
                     xytext=(right_line+text_width*len(end_event),top_line-start_time-duration),
                     horizontalalignment='right',
                     arrowprops=dict(arrowstyle="->",color="k",linestyle='dashed'))

def DrawArrowBack(start_time, duration, name, start_event, end_event):
    plt.arrow(right_line, top_line-start_time,-line_delta,-duration,
              head_width=0.1,head_length=0.1,length_includes_head=True,color='k')
    if name:
        plt.annotate(name,
                     xy=(left_line+line_delta/2,
                         text_float+top_line-start_time-duration/2),
                     horizontalalignment='center')
    if start_event:
        plt.annotate(start_event,
                     xy=(right_line,top_line-start_time),
                     xytext=(right_line+text_width*len(start_event),top_line-start_time),
                     horizontalalignment='right',
                     arrowprops=dict(arrowstyle="->",color="k",linestyle='dashed'))
    if end_event:
        plt.annotate(end_event,
                     xy=(left_line,top_line-start_time-duration),
                     xytext=(left_line-text_width*len(end_event),top_line-start_time-duration),
                     horizontalalignment='left',
                     arrowprops=dict(arrowstyle="->",color="k",linestyle='dashed'))

pp = PdfPages('app_2.pdf')

text_float = 0.1
line_delta = 0.5
bottom_line = 0
top_line = 10
text_width = 0.1
left_right_margin = 0.5

plt.annotate('c0',xy=(0, 10.2),horizontalalignment='center')
plt.annotate('s',xy=(0.5, 10.2),horizontalalignment='center')
plt.annotate('c1',xy=(1, 10.2),horizontalalignment='center')

left_line = 0
right_line = 0.5
drift = 5
plt.plot([left_line]*2,[bottom_line+drift,top_line],'k',linewidth=6)
plt.plot([right_line]*2,[bottom_line,top_line],'k',linewidth=6)
plt.annotate('client failure',
             xy=(left_line,
                 bottom_line+drift),
             xytext=(left_line-0.5,
                 bottom_line+drift),
             horizontalalignment='left',
             arrowprops=dict(arrowstyle="-",color="k",linestyle='dashed'))

DrawArrowFront(1, 0.2, 'tryAcquire', '', '')
DrawArrowBack(1.3, 0.2, '', '', '')
DrawArrowFront(2, 0.2, 'write "leader"', '', '')
DrawArrowBack(2.3, 0.2, '', '', '')


left_line = 0.5
right_line = 1
plt.plot([right_line]*2,[bottom_line,top_line],'k',linewidth=6)

DrawArrowBack(1.4, 0.2, 'tryAcquire', '', '')
DrawArrowFront(1.7, 0.2, '', '', '')
DrawArrowFront(2.5, 0.2, 'event(file)', '', '')
DrawArrowBack(3.0, 0.2, 'read: "leader"', '', '')
DrawArrowFront(3.3, 0.2, '', '', '')

DrawArrowFront(5, 0.2, 'event(lock)', '', '')

DrawArrowBack(5.5, 0.2, 'tryAcquire', '', '')
DrawArrowFront(5.8, 0.2, '', '', '')

DrawArrowBack(6.8, 0.2, 'write: "leader"', '', '')
DrawArrowFront(7.1, 0.2, '', '', '')


plt.axis('off')
left_line = 0
right_line = 1
plt.xlim(left_line-text_width-left_right_margin,
         right_line+text_width+left_right_margin)
plt.ylim(bottom_line, top_line+0.5)
plt.show();

#pp.savefig()
#pp.close()
