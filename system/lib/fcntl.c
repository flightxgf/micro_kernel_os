#include <fcntl.h>
#include <svc_call.h>
#include <ipc.h>
#include <vfs.h>
#include <stddef.h>

int open(const char* fname, int oflag) {
	int res = -1;
	fsinfo_t info;

	if(vfs_get(fname, &info) != 0)
		return -1;
	
	mount_t mount;
	if(vfs_get_mount(&info, &mount) != 0)
		return -1;
	
	proto_t in, out;
	proto_init(&in, NULL, 0);
	proto_init(&out, NULL, 0);

	proto_add_int(&in, FS_CMD_OPEN);
	proto_add(&in, &info, sizeof(fsinfo_t));
	proto_add_int(&in, oflag);

	if(ipc_call(mount.pid, &in, &out) == 0) {
		res = proto_read_int(&out);
	}

	proto_clear(&in);
	proto_clear(&out);
	return res;
}

void close(int fd) {
	fsinfo_t info;
	if(vfs_get_by_fd(fd, &info) != 0)
		return;
	
	mount_t mount;
	if(vfs_get_mount(&info, &mount) == 0) {
		proto_t in;
		proto_init(&in, NULL, 0);

		proto_add_int(&in, FS_CMD_CLOSE);
		proto_add(&in, &info, sizeof(fsinfo_t));

		ipc_call(mount.pid, &in, NULL);
		proto_clear(&in);
	}

	vfs_close(fd);
}
