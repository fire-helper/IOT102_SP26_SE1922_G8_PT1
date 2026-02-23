import type { PageServerLoad } from "./log/$types"

import { db } from "$lib/database"
import { fingerprintsTable } from "$lib/schemas"

export const load: PageServerLoad = async () => {
	return {
		fingerprints: await db.select().from(fingerprintsTable)
	}
}
