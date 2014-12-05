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
plt.plot([left_line]*2,[bottom_line,top_line],'k',linewidth=6)
plt.plot([right_line]*2,[bottom_line,top_line],'k',linewidth=6)

DrawArrowFront(1, 0.2, 'acquire', '', '')
DrawArrowBack(1.3, 0.2, '', '', '')
DrawArrowFront(2, 0.2, 'read: count', '', '')
DrawArrowBack(2.3, 0.2, '', '', '')
plt.annotate('n=0',xy=(left_line-0.15, top_line-0.05-2.5))
plt.annotate('n->1',xy=(left_line-0.15, top_line-0.05-3))
DrawArrowFront(3, 0.2, 'write: count', '', '')
plt.annotate('n=1',xy=(right_line+0.05, top_line-0.05-3.2))
DrawArrowBack(3.3, 0.2, '', '', '')
DrawArrowFront(4, 0.2, 'release', '', '')
DrawArrowBack(4.3, 0.2, '', '', '')

DrawArrowBack(8.3, 0.2, 'event', '', '')


left_line = 0.5
right_line = 1
plt.plot([right_line]*2,[bottom_line,top_line],'k',linewidth=6)

DrawArrowBack(1.2, 0.2, 'acquire', '', '')
DrawArrowFront(4.3, 0.2, '', '', '')
DrawArrowBack(5, 0.2, 'read: count', '', '')
DrawArrowFront(5.3, 0.2, '', '', '')
plt.annotate('n=1',xy=(right_line+0.05, top_line-0.05-5.5))
plt.annotate('n->2, triggered',xy=(right_line+0.05, top_line-0.05-6))
DrawArrowBack(6, 0.2, 'write: count', '', '')
plt.annotate('n=0',xy=(left_line-0.15, top_line-0.05-6.2))
DrawArrowFront(6.3, 0.2, '', '', '')
DrawArrowBack(7, 0.2, 'release', '', '')
DrawArrowFront(7.3, 0.2, '', '', '')

DrawArrowBack(8, 0.2, 'write: passed', '', '')
DrawArrowFront(8.3, 0.2, '', '', '')


plt.axis('off')
left_line = 0
right_line = 1
plt.xlim(left_line-text_width-left_right_margin,
         right_line+text_width+left_right_margin)
plt.ylim(bottom_line, top_line+0.5)
#plt.show();

pp.savefig()
pp.close()
