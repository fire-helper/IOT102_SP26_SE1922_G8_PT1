import { db } from "$lib/database"
import { fingerprintsTable } from "$lib/schemas"

import { error, type RequestHandler } from "@sveltejs/kit"

var hasRequestEnqueued = false

export const POST: RequestHandler = async (req) => {
	hasRequestEnqueued = true
	return new Response()
}

export const PUT: RequestHandler = async (req) => {
	console.log(req)

	const timestamp = new Date(Date.now())
	const fingerprintObject: typeof fingerprintsTable.$inferInsert = {
		id: 0,
		registerTimestamp: timestamp.toISOString()
	}
	await db.update(fingerprintsTable).set(fingerprintObject)

	return new Response()
}

export const GET: RequestHandler = async () => {
	if (hasRequestEnqueued) {
		hasRequestEnqueued = false
		return new Response()
	}

	return error(403)
}
