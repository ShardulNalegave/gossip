package packets

type MessageKind uint8

const (
	GossipMessageKind       MessageKind = 0
	SendAllPeersMessageKind MessageKind = 1
)

type MessageHeader struct {
	Kind   MessageKind
	Length uint32
}
