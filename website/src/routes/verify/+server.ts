import { error, type RequestHandler } from "@sveltejs/kit"

var hasRequestEnqueued = false

export const POST: RequestHandler = async (req) => {
	hasRequestEnqueued = true
	return new Response()
}

export const GET: RequestHandler = async () => {
	if (hasRequestEnqueued) {
		hasRequestEnqueued = false
		return new Response()
	}

	return error(403)
}
