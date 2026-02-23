import { int, sqliteTable, text } from "drizzle-orm/sqlite-core"

export const logsTable = sqliteTable("logs_table", {
	id: int().primaryKey({ autoIncrement: true }).notNull(),
	text: text().notNull(),
	timestamp: text().notNull()
})

export const fingerprintsTable = sqliteTable("fingerprints_table", {
	id: int().primaryKey({ autoIncrement: true }).notNull(),
	registerTimestamp: text().notNull()
})

export const authAttemptsTable = sqliteTable("auth_attempts_table", {
	id: int().primaryKey({ autoIncrement: true }).notNull(),
	success: int().notNull(),
	timestamp: text().notNull()
})
