#include <stdio.h>
#include "WearhouseManager.h"

Package* create_package(long priority, const char* destination) {
	Package* package = (Package*)malloc(sizeof(Package));
	if (package == NULL) {
		printf("eroare alocare");
		return NULL;
	}
	package->priority = priority;
	if (destination != NULL) {
		package->destination = strdup(destination);
	} else {
		package->destination = NULL;
	}
	return package;
}

void destroy_package(Package* package) {
	if (package == NULL) return;
	if (package->destination != NULL) free(package->destination);
	free(package);
}

Manifest* create_manifest_node(void) {
	Manifest* node = (Manifest*)malloc(sizeof(Manifest));
	if (node == NULL) {
		printf("eroare alocare");
		return NULL;
	}
	node->next = node->prev = NULL;
	node->package = NULL;
	return node;
}

void destroy_manifest_node(Manifest* manifest_node) {
	if (manifest_node == NULL) return;
	if (manifest_node->package != NULL) destroy_package(manifest_node->package);
	free(manifest_node);
}

Wearhouse* create_wearhouse(long capacity) {
	if (capacity <= 0) {
		return NULL;
	}
	Wearhouse* w = (Wearhouse*)malloc(sizeof(Wearhouse));
	if (w == NULL) {
		printf("eroare alocare");
		return NULL;
	}
	w->capacity = capacity;
	w->size = 0;
	w->packages = (Package**)malloc(sizeof(Package*) * capacity);
	if (w->packages == NULL) {
		printf("eroare alocare");
		free(w);
		return NULL;
	}
	return w;
}

Wearhouse* open_wearhouse(const char* file_path) {
	size_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse* w = NULL;


	FILE* fp = fopen(file_path, "r");
	if (fp == NULL)
		goto file_open_exception;

	if ((read_size = getline(&line, &len, fp)) != -1) {
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while ((read_size = getline(&line, &len, fp)) != -1) {
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package* p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse* w) {
	return (w->size == 0);
}

long wearhouse_is_full(Wearhouse* w) {
	return (w->size == w->capacity);
}

long wearhouse_max_package_priority(Wearhouse* w) {
	if (wearhouse_is_empty(w) == 0) {
		long max = -1;
			for (int i = 0; i < w->size; i++) {
				if (w->packages[i]->priority > max) {
					max = w->packages[i]->priority;
				}
			}
		return max;
	}	else {
		return 0;
	}
}

long wearhouse_min_package_priority(Wearhouse* w) {
	if (wearhouse_is_empty(w) == 0) {
		long min = 10;
		for (int i = 0; i < w->size; i++) {
			if (w->packages[i]->priority < min) {
				min = w->packages[i]->priority;
			}
		}
		return min;
	}	else {
		return 0;
	}
}


void wearhouse_print_packages_info(Wearhouse* w) {
	for (long i = 0; i < w->size; i++) {
		printf("P: %ld %s\n",
			w->packages[i]->priority,
			w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse) {
	for (int i = 0; i < wearhouse->size; i++) {
		destroy_package(wearhouse->packages[i]);
	}
	free(wearhouse->packages);
	free(wearhouse);
}


Robot* create_robot(long capacity) {
	if (capacity < 0 ){
		return NULL;
	}
	Robot* r = (Robot*)malloc(sizeof(Robot));
	if (r == NULL) {
		printf("eroare alocare");
		return NULL;
	}
	r->manifest = NULL;
	r->next = NULL;
	r->size = 0;
	r->capacity = capacity;
	return r;
}

int robot_is_full(Robot* robot) {
	return (robot->size == robot->capacity);
}

int robot_is_empty(Robot* robot) {
	return (robot->size == 0);
}

Package* robot_get_wearhouse_priority_package(Wearhouse* w, long priority) {
	if (priority < 0) return NULL;
	for (int i = 0; i < w->size; i++) {
		if (w->packages[i]->priority == priority) {
			return w->packages[i];
		}
	}
	return NULL; // if not found
}

void robot_remove_wearhouse_package(Wearhouse* w, Package* package) {
	if (package == NULL) return;
	int k = 0;
	while (w->packages[k] != package) k++;
	for (int j = k; j < w->size - 1; j++) {
		w->packages[j] = w->packages[j+1];
	}
	w->size--;
}

void insertToLeft(Manifest* it, Manifest* node, Robot* robot) {
	if (it->prev == NULL) { // corner case: beginning of manifest
		node->next = it;
		it->prev = node;
		robot->manifest = node;
	} else {
	node->next = it;
	node->prev = it->prev;
	it->prev->next = node;
	it->prev = node;
	}
	robot->size++;
}

void insertToEnd(Manifest* it, Manifest* node, Robot* robot){
	it->next = node;
	node->prev = it;
	robot->size++;
}

void robot_load_one_package(Robot* robot, Package* package) {
	if (package == NULL || robot == NULL || robot_is_full(robot) == 1) return;
	Manifest* node = create_manifest_node();
	node->package = package;
	if (robot_is_empty(robot)) { // corner case: empty robot
		robot->manifest = node;
		robot->size++;
	} else {
		Manifest* it = robot->manifest;
		while (it != NULL) {
			if (it->package->priority < package->priority) {
				insertToLeft(it, node, robot);
				return;
			} else if (it->package->priority > package->priority) {
				if (it->next == NULL) { // corner case: end of manifest
					insertToEnd(it, node, robot);
					return;
				} else it = it->next;
			} else if (strcmp(it->package->destination, package->destination) > 0) { // if priorities are equal
				insertToLeft(it, node, robot);
				return;
			} else if (it->next == NULL) { // corner case: end of manifest
				insertToEnd(it, node, robot);
				return;
			} else it = it->next;
		}
	}
}

long robot_load_packages(Wearhouse* wearhouse, Robot* robot) {
	long load = 0, first;
	Package* package;
	while (!robot_is_full(robot) && !wearhouse_is_empty(wearhouse)) {
		first = wearhouse_max_package_priority(wearhouse);
		package = robot_get_wearhouse_priority_package(wearhouse, first);
		robot_load_one_package(robot, package);
		robot_remove_wearhouse_package(wearhouse, package);
		load++;
	}
	return load;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination) {
	if (robot_is_empty(robot)) return NULL;
	long max = -1;
	Package* package = NULL;
	for (int i = 0; i < robot->size; i++) {
		if (robot->manifest->package->priority > max) {
			max = robot->manifest->package->priority;
			package = robot->manifest->package;
		}
	}
	return package;
}

void destroy_robot(Robot* robot) {
	if (robot == NULL) return;
	Manifest* it = robot->manifest;
	Manifest* aux;
	while (it != NULL) {
		aux = it;
		it = it->next;
		destroy_manifest_node(aux);
	}
	free(robot);
}

void robot_unload_package(Truck* truck, Robot* robot, Manifest* node_r) {
	Manifest* node_t = create_manifest_node();
	node_t->package = node_r->package;
	Manifest* it_t = truck->manifest; // iterator for truck
	if (it_t == NULL) { // corner case: empty truck
		truck->manifest = node_t;
	} else {
		while (it_t->next != NULL) it_t = it_t->next;
		it_t->next = node_t;
		node_t->prev = it_t;
	}
	truck->size++;
	if (node_r->prev == NULL && node_r->next == NULL) { // corner case: only 1 element
		robot->manifest = NULL;
		robot->size--;
		return;
	}
	if (node_r->prev == NULL && node_r->next != NULL) { // corner case: beginning of list
		node_r->next->prev = NULL;
		robot->manifest = node_r->next;
		robot->size--;
		return;
	}
	if (node_r->prev != NULL && node_r->next == NULL) { // corner case: end of list
		node_r->prev->next = NULL;
		robot->size--;
		return;
	}
	// normal case
	node_r->next->prev = node_r->prev;
	node_r->prev->next = node_r->next;
	robot->size--;
}

void robot_unload_packages(Truck* truck, Robot* robot) {
	if (truck == NULL || robot == NULL || robot_is_empty(robot)) return;
	Manifest* it_r = robot->manifest; // iterator for robot
	while (!truck_is_full(truck) && it_r != NULL) {
		if (strcmp(it_r->package->destination, truck->destination) == 0) {
			robot_unload_package(truck, robot, it_r);
		}
		it_r = it_r->next;
	}
}


// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot* parkinglot) {
	int found_truck = 0;
	long size = 0;
	Truck* arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while (m_iterator != NULL) {
		while (arrived_iterator != parkinglot->arrived_trucks) {
			size = truck_destination_robots_unloading_size(arrived_iterator);
			if (strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
				size < (arrived_iterator->capacity - arrived_iterator->size)) {
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if (found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if (found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while (r_iterator != NULL) {
		Package* pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if (m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if (prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot) {
	Manifest* iterator = robot->manifest;
	while (iterator != NULL) {
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}

Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time) {
	Truck* truck = (Truck*) malloc(sizeof(Truck));
	if (truck == NULL) {
		printf("eroare alocare");
		return NULL;
	}
	truck->manifest = NULL;
	truck->unloading_robots = NULL;
	if (destination != NULL) {
		truck->destination = strdup(destination);
	} else {
		truck->destination = NULL;
	}
	truck->size = 0;
	truck->capacity = capacity;
	truck->in_transit_time = 0;
	truck->transit_end_time = transit_time;
	truck->departure_time = departure_time;
	truck->next = NULL;
	return truck;
}

int truck_is_full(Truck* truck) {
	return (truck->size == truck->capacity);
}

int truck_is_empty(Truck* truck) {
	return (truck->size == 0);
}

long truck_destination_robots_unloading_size(Truck* truck) {
	long unload_size = 0;
	Robot* r = truck->unloading_robots;
	Manifest* it;
	while (r != NULL) {
		it = r->manifest;
		while (it != NULL) {
			if (strcmp(it->package->destination, truck->destination) == 0) {
				unload_size += r->size;
				break;
			}
			it = it->next;
		}
		r = r->next;
	}
	return unload_size;
}


void truck_print_info(Truck* truck) {
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
		truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while (m_iterator != NULL) {
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while (r_iterator != NULL) {
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck) {
	if (truck == NULL) return;
	Manifest* it1 = truck->manifest;
	Manifest* aux1;
	while (it1 != NULL) {
		aux1 = it1;
		it1 = it1->next;
		destroy_manifest_node(aux1);
	}
	Robot* it2 = truck->unloading_robots;
	Robot* aux2;
	while (it2 != NULL) {
		aux2 = it2;
		it2 = it2->next;
		destroy_robot(aux2);
	}
	free(truck);
}


Parkinglot* create_parkinglot(void) {
	Parkinglot* lot = (Parkinglot*) malloc(sizeof(Parkinglot));
	if (lot == NULL) {
		printf("eroare alocare");
		return NULL;
	}
	lot->arrived_trucks = create_truck(NULL, -1, -1, -1);
	lot->departed_trucks = create_truck(NULL, -1, -1, -1);
	lot->pending_robots = create_robot(0);
	lot->standby_robots = create_robot(0);

	lot->arrived_trucks->next =  lot->arrived_trucks;
	lot->departed_trucks->next = lot->departed_trucks;
	lot->pending_robots->next = lot->pending_robots;
	lot->standby_robots->next = lot->standby_robots;

	// santinel rules
	lot->arrived_trucks->size =  -1;
	lot->departed_trucks->size =  -1;
	lot->departed_trucks->departure_time = -1;
	lot->pending_robots->size = -1;
	lot->standby_robots->capacity = -1;
	return lot;
}

Parkinglot* open_parckinglot(const char* file_path) {
	size_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot* parkinglot = create_parkinglot();

	FILE* fp = fopen(file_path, "r");
	if (fp == NULL)
		goto file_open_exception;

	while ((read_size = getline(&line, &len, fp)) != -1) {
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if (token[0] == 'T') {
			token = strtok(NULL, ",\n ");
			char* destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck* truck = create_truck(destination, capacity, transit_time, departure_time);

			if (arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}
		else if (token[0] == 'R') {
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot* robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot* robot) {
	if (parkinglot == NULL || robot == NULL) return;
	if (robot_is_empty(robot)) {
		Robot* it = parkinglot->standby_robots;
		while (it->capacity > robot->capacity && it->next != parkinglot->standby_robots) it = it->next;
		Robot* temp = it->next;
		it->next = robot;
		it->next->next = temp;
	} else {
		Robot* it = parkinglot->pending_robots;
		while (it->size > robot->size && it->next != parkinglot->pending_robots) it = it->next;
		Robot* temp = it->next;
		it->next = robot;
		it->next->next = temp;
	}
 }

void parkinglot_remove_robot(Parkinglot* parkinglot, Robot* robot) {
	if (parkinglot == NULL || robot == NULL) return;
	if (robot_is_empty(robot)) {
		Robot* it = parkinglot->standby_robots;
		while (it->next != robot) it = it->next;
		it->next = it->next->next;
	} else {
		Robot* it = parkinglot->pending_robots;
		while (it->next != robot) it = it->next;
		it->next = it->next->next;
	}
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot) {
	return (parkinglot->pending_robots->next->size != -1);
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot) {
	Truck* it = parkinglot->arrived_trucks->next;
	while (it->size != -1) {
		if (it->size != 0) return 0;
		it = it->next;
	}
	return 1;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot) {
	return (parkinglot->departed_trucks->next->departure_time != -1);
}

void freeTruckList (Truck* head) {
	if (head == NULL) return;
	Truck* current = head;
	while (current->next != head) {
		Truck* aux = head->next;
		head = head->next;
		destroy_truck(aux);
	}
}

void freeRobotList (Robot* head) {
	if (head == NULL) return;
	Robot* current = head;
	while (current->next != head) {
		Robot* aux = head->next;
		head = head->next;
		destroy_robot(aux);
	}
}

void destroy_parkinglot(Parkinglot* parkinglot) {
	if (parkinglot == NULL) return;
	freeRobotList(parkinglot->pending_robots);
	freeRobotList(parkinglot->standby_robots);
	freeTruckList(parkinglot->arrived_trucks);
	freeTruckList(parkinglot->departed_trucks);
	free(parkinglot);
}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot) {
	Truck* iterator = parkinglot->arrived_trucks->next;
	while (iterator != parkinglot->arrived_trucks) {

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot) {
	Truck* iterator = parkinglot->departed_trucks->next;
	while (iterator != parkinglot->departed_trucks) {
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot) {
	Robot* iterator = parkinglot->pending_robots->next;
	while (iterator != parkinglot->pending_robots) {
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot) {
	Robot* iterator = parkinglot->standby_robots->next;
	while (iterator != parkinglot->standby_robots) {
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void removeFromList(Truck* it, Truck* truck) {
	int found = 1;
	while (it->next != truck) {
		if (it->next->size == -1) {
			found = 0;
			break;
		}
		it = it->next;
	}
	if (found == 1) it->next = it->next->next;
}

void truck_departed(Parkinglot* parkinglot, Truck* truck) {
	if (parkinglot == NULL || truck == NULL) return;

	Truck* it = parkinglot->arrived_trucks;
	removeFromList(it, truck);

	it = parkinglot->departed_trucks;
	while (truck->departure_time > it->next->departure_time && it->next->departure_time != -1) it = it->next;
	Truck* temp = it->next;
	it->next = truck;
	it->next->next = temp;
}


void truck_arrived(Parkinglot* parkinglot, Truck* truck) {
	if (parkinglot == NULL || truck == NULL) return;

	truck->size = 0;
	truck->manifest = NULL;
	truck->in_transit_time = 0;

	Truck* it = parkinglot->departed_trucks;
	removeFromList(it, truck);

	it = parkinglot->arrived_trucks;

	while (it->next != parkinglot->arrived_trucks &&
		strcmp(truck->destination, it->next->destination) > 0) it = it->next;
	while (it->next != parkinglot->arrived_trucks &&
		strcmp(truck->destination, it->next->destination) == 0 &&
		truck->departure_time > it->next->departure_time) it = it->next;
	Truck* temp = it->next;
	it->next = truck;
	it->next->next = temp;
}

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck) {
	Robot* it = truck->unloading_robots;
	Robot* temp;
	while (it != NULL) {
		temp = it->next;
		parkinglot_add_robot(parkinglot, it);
		it = temp;
	}
	truck->unloading_robots = NULL;
}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour) {
	Truck* it = parkinglot->arrived_trucks;
	while (it->next != parkinglot->arrived_trucks) {
		if (it->next->departure_time <= day_hour) truck_departed(parkinglot, it->next);
		it = it->next;
	}
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot) {
	Truck* it = parkinglot->departed_trucks;
	while (it->next != parkinglot->departed_trucks) {
		if (it->next->in_transit_time < it->next->transit_end_time - 1) it->next->in_transit_time++;
		else truck_arrived(parkinglot, it->next);
		it = it->next;
	}
}

void robot_swarm_collect(Wearhouse* wearhouse, Parkinglot* parkinglot) {
	Robot* head_robot = parkinglot->standby_robots;
	Robot* current_robot = parkinglot->standby_robots->next;
	while (current_robot != parkinglot->standby_robots) {

		// Load packages from wearhouse if possible
		if (!robot_load_packages(wearhouse, current_robot)) {
			break;
		}

		// Remove robot from standby list
		Robot* aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot* parkinglot) {

	Robot* current_robot = parkinglot->pending_robots->next;

	while (current_robot != parkinglot->pending_robots) {
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if (!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot) {
	Truck* arrived_iterator = parkinglot->arrived_trucks->next;
	while (arrived_iterator != parkinglot->arrived_trucks) {
		Robot* current_robot = arrived_iterator->unloading_robots;
		while (current_robot != NULL) {
			robot_unload_packages(arrived_iterator, current_robot);
			Robot* aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}
