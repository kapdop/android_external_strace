#include <stdio.h>
#include <errno.h>
#include <sys/shm.h>

int
main(void)
{
	int rc, id;
	struct shmid_ds ds;

	id = shmget(IPC_PRIVATE, 1, 0600);
	if (id < 0)
		return 77;
	printf("shmget\\(IPC_PRIVATE, 1, 0600\\) += %d\n", id);

	if (shmctl(id, IPC_STAT, &ds))
		goto fail;
	printf("shmctl\\(%d, (IPC_64\\|)?IPC_STAT, \\{shm_perm=\\{uid=%u, gid=%u, "
		"mode=%#o, key=%u, cuid=%u, cgid=%u\\}, shm_segsz=%u, shm_cpid=%u, "
		"shm_lpid=%u, shm_nattch=%u, shm_atime=%u, shm_dtime=%u, "
		"shm_ctime=%u\\}\\) += 0\n",
		id, (unsigned) ds.shm_perm.uid, (unsigned) ds.shm_perm.gid,
		(unsigned) ds.shm_perm.mode, (unsigned) ds.shm_perm.__key,
		(unsigned) ds.shm_perm.cuid, (unsigned) ds.shm_perm.cgid,
		(unsigned) ds.shm_segsz, (unsigned) ds.shm_cpid,
		(unsigned) ds.shm_lpid, (unsigned) ds.shm_nattch,
		(unsigned) ds.shm_atime, (unsigned) ds.shm_dtime,
		(unsigned) ds. shm_ctime);

	int max = shmctl(0, SHM_INFO, &ds);
	if (max < 0)
		goto fail;
	printf("shmctl\\(0, (IPC_64\\|)?SHM_INFO, %p\\) += %d\n", &ds, max);

	rc = shmctl(id, SHM_STAT, &ds);
	if (rc != id) {
		/*
		 * In linux < v2.6.24-rc1 the first argument must be
		 * an index in the kernel's internal array.
		 */
		if (-1 != rc || EINVAL != errno)
			goto fail;
		printf("shmctl\\(%d, (IPC_64\\|)?SHM_STAT, %p\\) += -1 EINVAL \\(Invalid argument\\)\n", id, &ds);
	} else {
		printf("shmctl\\(%d, (IPC_64\\|)?SHM_STAT, %p\\) += %d\n", id, &ds, id);
	}

	rc = 0;
done:
	if (shmctl(id, IPC_RMID, NULL) < 0)
		return 1;
	printf("shmctl\\(%d, (IPC_64\\|)?IPC_RMID, NULL\\) += 0\n", id);
	return rc;

fail:
	rc = 1;
	goto done;
}
