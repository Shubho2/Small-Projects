import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk


class Test(object):
	vehicle_capacity = []
	node = []
	node_type = []
	ntype = 1
	time_window = []

	def __init__(self, vehicle_capacity,node,node_type,time_window):
		super(Test, self).__init__()
		self.vehicle_capacity = vehicle_capacity
		self.node = node
		self.node_type = node_type
		self.time_window = time_window
	def func(self):
		print "called after init"
		for x in xrange(0,len(self.node)):
			print self.node[x]

class LabelWindow(Gtk.Window):

	vehicle_capacity = []
	node = []
	node_type = []
	ntype = 1
	time_window = []
	depot_set = 0

	def __init__(self):
	    Gtk.Window.__init__(self, title="Create Graph")
	    hbox = Gtk.Box(spacing=10)
	    hbox.set_homogeneous(False)

	    hboxTop = Gtk.Box(spacing=1)
	    hboxSecondTop = Gtk.Box(spacing=1)
	    hboxMiddle = Gtk.Box(spacing=1)
	    hboxSecondBottom = Gtk.Box(spacing=1)
	    hboxBottom = Gtk.Box(spacing=1)

	    container_vbox_inside_hbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    container_vbox_inside_hbox.set_homogeneous(False)

	    hbox.pack_start(container_vbox_inside_hbox,True,True,0)

	    container_vbox_inside_hbox.pack_start(hboxTop, True, True, 0)
	    container_vbox_inside_hbox.pack_start(hboxSecondTop, True, True, 0)
	    container_vbox_inside_hbox.pack_start(hboxMiddle, True, True, 0)
	    container_vbox_inside_hbox.pack_start(hboxSecondBottom, True, True, 0)
	    container_vbox_inside_hbox.pack_start(hboxBottom, True, True, 0)

	    # add capacity in the second top horizontal box
	    capacity_left = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    capacity_left.set_homogeneous(False)
	    capacity_right = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    capacity_right.set_homogeneous(False)
	    
	    hboxTop.pack_start(capacity_left, True, True, 0)
	    hboxTop.pack_start(capacity_right, True, True, 0)

	    # add vehicle capacity
	    label = Gtk.Label("Vehicle capacity")
	    label.set_justify(Gtk.Justification.LEFT)
	    capacity_left.pack_start(label, True, True, 0)

	    # add entry for x-cordinate
	    self.capacity = Gtk.Entry()
	    self.capacity.set_text("20")
	    capacity_right.pack_start(self.capacity,True,True,0)

	    # add coordinates in the middle horizontal box

	    vbox_left = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    vbox_left.set_homogeneous(False)
	    vbox_right = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    vbox_right.set_homogeneous(False)
	    
	    hboxMiddle.pack_start(vbox_left, True, True, 0)
	    hboxMiddle.pack_start(vbox_right, True, True, 0)

	    hboxInvbox_left = Gtk.Box(spacing=10)
	    hboxInvbox_left.set_homogeneous(False)

	    vbox_left.pack_start(hboxInvbox_left,True,True,0)

	    corodinate_label = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    corodinate_label.set_homogeneous(False)
	    cordinate_entry = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    cordinate_entry.set_homogeneous(False)
	    
	    hboxInvbox_left.pack_start(corodinate_label, True, True, 0)
	    hboxInvbox_left.pack_start(cordinate_entry, True, True, 0)

	    label = Gtk.Label("X-coordinate")
	    label.set_justify(Gtk.Justification.LEFT)
	    corodinate_label.pack_start(label, True, True, 0)

	    # add entry for x-cordinate
	    self.xentry = Gtk.Entry()
	    self.xentry.set_text("0")
	    cordinate_entry.pack_start(self.xentry,True,True,0)
	    
	    label = Gtk.Label("Y-coordinate")
	    label.set_justify(Gtk.Justification.LEFT)
	    corodinate_label.pack_start(label, True, True, 0)
	    
	    # add entry for y-cordinate
	    self.yentry = Gtk.Entry()
	    self.yentry.set_text("0")
	    cordinate_entry.pack_start(self.yentry,True,True,0)

	    # add radio button in vbox right of middle horizontal box
	    self.radio_customer = Gtk.RadioButton.new_with_label_from_widget(None,"Customer")
	    self.radio_customer.connect("toggled", self.on_customer_toggled)
	    vbox_right.pack_start(self.radio_customer, True, True, 0)

	    if not self.depot_set:
		    self.radio_disposal = Gtk.RadioButton.new_with_label_from_widget(self.radio_customer,"Disposal")
		    self.radio_disposal.connect("toggled", self.on_disposal_toggled)
		    vbox_right.pack_start(self.radio_disposal, True, True, 0)

	    self.radio_depot = Gtk.RadioButton.new_with_label_from_widget(self.radio_customer,"Depot")
	    self.radio_depot.connect("toggled", self.on_depot_toggled)
	    vbox_right.pack_start(self.radio_depot, True, True, 0)


	    # add time window 
	    time_left = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    time_left.set_homogeneous(False)
	    time_middle = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    time_middle.set_homogeneous(False)
	    time_right = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    time_right.set_homogeneous(False)
	    
	    hboxSecondBottom.pack_start(time_left, True, True, 0)
	    hboxSecondBottom.pack_start(time_middle, True, True, 0)
	    hboxSecondBottom.pack_start(time_right, True, True, 0)
	   
	    
	    window_label = Gtk.Label("Time window")
	    window_label.set_justify(Gtk.Justification.LEFT)
	    time_left.pack_start(window_label, True, True, 0)

	    # add entry for start time
	    self.start_time = Gtk.Entry()
	    self.start_time.set_text("0")
	    time_middle.pack_start(self.start_time,True,True,0)
	    
	    # add entry for end time
	    self.end_time = Gtk.Entry()
	    self.end_time.set_text("0")
	    time_right.pack_start(self.end_time,True,True,0)

	    # add buttons 
	    node_button = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    node_button.set_homogeneous(False)
	    animate_button = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=10)
	    animate_button.set_homogeneous(False)
	    
	    hboxBottom.pack_start(node_button, True, True, 0)
	    hboxBottom.pack_start(animate_button, True, True, 0)
	   
	    
	    bnode = Gtk.Button.new_with_mnemonic("Add the Node")
	    bnode.connect("clicked", self.on_add_node_clicked)
	    node_button.pack_start(bnode,True,True,0)

	    animate = Gtk.Button.new_with_mnemonic("Call Animator")
	    animate.connect("clicked", self.on_animator_clicked)
	    animate_button.pack_start(animate,True,True,0)

	    self.add(hbox)

	def on_customer_toggled(self):
		self.ntype = 1
		
	def on_disposal_toggled(self):
		self.ntype = 2

	def on_depot_toggled(self):
		self.ntype = 3
		self.depot_set = 1

	def on_add_node_clicked(self,bnode):
		self.vehicle_capacity.append(int(self.capacity.get_text()))
		self.node.append([int(self.xentry.get_text()),int(self.yentry.get_text())])
		self.node_type.append(self.ntype)
		self.time_window.append([int(self.start_time.get_text()),int(self.end_time.get_text())])

	def on_animator_clicked(self,animate):
		testObj = Test(self.vehicle_capacity, self.node, self.node_type,self.time_window)
		testObj.func()

window = LabelWindow()        
window.connect("destroy", Gtk.main_quit)
window.show_all()
Gtk.main()
