package packets

type PeerStatus uint8

const (
	AlivePeerStatus   PeerStatus = 0
	FailingPeerStatus PeerStatus = 1
	DeadPeerStatus    PeerStatus = 2
)

type PeerRecord struct {
	Status           PeerStatus
	SockAddr         [8]byte // sizeof(struct sockaddr_in)
	LastCommunicated int64
	LastUpdated      int64
}
