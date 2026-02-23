import type { RequestHandler } from "@sveltejs/kit"

import { db } from "$lib/database"
import { logsTable } from "$lib/schemas"

export const POST: RequestHandler = async (req) => {
	const logMessage = await req.request.text()
	console.log(logMessage)

	const timestamp = new Date(Date.now())
	const logObject: typeof logsTable.$inferInsert = {
		text: logMessage,
		timestamp: timestamp.toISOString()
	}
	await db.insert(logsTable).values(logObject)

	return new Response()
}
