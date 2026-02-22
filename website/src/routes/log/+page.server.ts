import type { PageServerLoad } from "./$types"

import { db } from "$lib/database"
import { logsTable } from "$lib/schemas"

export const load: PageServerLoad = async () => {
	const logs = await db.select().from(logsTable)
	return {
		logs: logs
	}
}
